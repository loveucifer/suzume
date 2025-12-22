#include "FirstApp.hpp"
#include "SuzumeModel.hpp"
#include "SuzumePipeline.hpp"
#include "SuzumeSwapChain.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/constants.hpp"
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>
#include <vulkan/vulkan_core.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

namespace Suzume {

struct SuzumePushConstantData {
  glm::mat2 transform{1.0f}; // default @ identity matrix
  glm::vec2 offset;          // 8 bytes at offset 0
  alignas(16) glm::vec3
      color; // 12 bytes at offset 16 (vec3 requires 16-byte alignment in GLSL)
};

FirstApp::FirstApp() {
  loadGameObjects();

  createPipelineLayout();
  createPipeline();
}

FirstApp::~FirstApp() {
  vkDestroyPipelineLayout(suzumeDevice.device(), pipelineLayout, nullptr);
}

void FirstApp::run() {
  while (!suzumeWindow.shouldClose()) {
    glfwPollEvents();
    if (auto commandBuffer = suzumeRenderer.beginFrame()) {
      suzumeRenderer.beginSwapChainRenderPass(commandBuffer);
      renderGameObjects(commandBuffer);
      suzumeRenderer.endSwapChainRenderPass(commandBuffer);
      suzumeRenderer.endFrame();
    }
  }

  vkDeviceWaitIdle(suzumeDevice.device());
}

void FirstApp::loadGameObjects() {
  std::vector<SuzumeModel::Vertex> vertices{
      {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
      {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
      {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
  auto suzumeModel = std::make_shared<SuzumeModel>(
      suzumeDevice, vertices); // unique before but now thats not the case
  auto triangle = SuzumeGameObject::create();
  triangle.model = suzumeModel;
  triangle.color = {0.1f, 0.8f, 0.1f};
  triangle.transform2d.translations.x = 0.2f;
  triangle.transform2d.scale = {2.0f, 0.5f};
  triangle.transform2d.rotation =
      .25f *
      glm::two_pi<float>(); // note to git diff we are using radians not degrees
  gameObjects.push_back(std::move(triangle));
}

// note to self -y axis in vulkan is up , you already made this mistake
void FirstApp::createPipelineLayout() {

  VkPushConstantRange pushConstantRange{};
  pushConstantRange.stageFlags =
      VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
  pushConstantRange.offset = 0;
  pushConstantRange.size = sizeof(SuzumePushConstantData);

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.pushConstantRangeCount = 1;
  pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
  if (vkCreatePipelineLayout(suzumeDevice.device(), &pipelineLayoutInfo,
                             nullptr, &pipelineLayout) != VK_SUCCESS) {
    throw std::runtime_error("failed to create pipeline layout!");
  }
}

void FirstApp::createPipeline() {
  PipelineConfigInfo pipelineConfig{};
  SuzumePipeline::defaultPipelineConfigInfo(pipelineConfig);
  pipelineConfig.renderPass = suzumeRenderer.getSwapChainRenderPass();
  pipelineConfig.pipelineLayout = pipelineLayout;
  suzumePipeline = std::make_unique<SuzumePipeline>(
      suzumeDevice, "shaders/SuzumeShader.vert.spv",
      "shaders/SuzumeShader.frag.spv", pipelineConfig);
}

void FirstApp::renderGameObjects(VkCommandBuffer commandBuffer) {
  suzumePipeline->bind(commandBuffer);
  // updater for testing shit
  int i = 0;
  for (auto &obj : gameObjects) {
    i += 1;
    obj.transform2d.rotation = glm::mod<float>(
        obj.transform2d.rotation + 0.001f * i, 2.f * glm::pi<float>());
  }
  // renderer

  for (auto &obj : gameObjects) {
    SuzumePushConstantData push{};
    push.offset = obj.transform2d.translations;
    push.color = obj.color;
    push.transform = obj.transform2d.mat2();

    vkCmdPushConstants(commandBuffer, pipelineLayout,
                       VK_SHADER_STAGE_VERTEX_BIT |
                           VK_SHADER_STAGE_FRAGMENT_BIT,
                       0, sizeof(SuzumePushConstantData), &push);
    obj.model->bind(commandBuffer);
    obj.model->draw(commandBuffer);
  }
}

} // namespace Suzume
