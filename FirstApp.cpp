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
  glm::mat4 transform{1.f};
  alignas(16) glm::vec3 color{};
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

// temporary helper function, creates a 1x1x1 cube centered at offset
std::unique_ptr<SuzumeModel> createCubeModel(SuzumeDevice &device, glm::vec3 offset) {
  std::vector<SuzumeModel::Vertex> vertices{

      // left face (coral pink gradient)
      {{-.5f, -.5f, -.5f}, {1.0f, .4f, .5f}},
      {{-.5f, .5f, .5f}, {1.0f, .6f, .7f}},
      {{-.5f, -.5f, .5f}, {1.0f, .5f, .6f}},
      {{-.5f, -.5f, -.5f}, {1.0f, .4f, .5f}},
      {{-.5f, .5f, -.5f}, {1.0f, .5f, .6f}},
      {{-.5f, .5f, .5f}, {1.0f, .6f, .7f}},

      // right face (mint green gradient)
      {{.5f, -.5f, -.5f}, {.3f, .9f, .7f}},
      {{.5f, .5f, .5f}, {.5f, 1.0f, .8f}},
      {{.5f, -.5f, .5f}, {.4f, .95f, .75f}},
      {{.5f, -.5f, -.5f}, {.3f, .9f, .7f}},
      {{.5f, .5f, -.5f}, {.4f, .95f, .75f}},
      {{.5f, .5f, .5f}, {.5f, 1.0f, .8f}},

      // top face (cyan gradient)
      {{-.5f, -.5f, -.5f}, {.2f, .8f, 1.0f}},
      {{.5f, -.5f, .5f}, {.4f, .9f, 1.0f}},
      {{-.5f, -.5f, .5f}, {.3f, .85f, 1.0f}},
      {{-.5f, -.5f, -.5f}, {.2f, .8f, 1.0f}},
      {{.5f, -.5f, -.5f}, {.3f, .85f, 1.0f}},
      {{.5f, -.5f, .5f}, {.4f, .9f, 1.0f}},

      // bottom face (magenta gradient)
      {{-.5f, .5f, -.5f}, {.9f, .2f, .6f}},
      {{.5f, .5f, .5f}, {1.0f, .4f, .8f}},
      {{-.5f, .5f, .5f}, {.95f, .3f, .7f}},
      {{-.5f, .5f, -.5f}, {.9f, .2f, .6f}},
      {{.5f, .5f, -.5f}, {.95f, .3f, .7f}},
      {{.5f, .5f, .5f}, {1.0f, .4f, .8f}},

      // front face (electric blue gradient)
      {{-.5f, -.5f, 0.5f}, {.1f, .4f, 1.0f}},
      {{.5f, .5f, 0.5f}, {.3f, .6f, 1.0f}},
      {{-.5f, .5f, 0.5f}, {.2f, .5f, 1.0f}},
      {{-.5f, -.5f, 0.5f}, {.1f, .4f, 1.0f}},
      {{.5f, -.5f, 0.5f}, {.2f, .5f, 1.0f}},
      {{.5f, .5f, 0.5f}, {.3f, .6f, 1.0f}},

      // back face (purple gradient)
      {{-.5f, -.5f, -0.5f}, {.6f, .2f, .9f}},
      {{.5f, .5f, -0.5f}, {.8f, .4f, 1.0f}},
      {{-.5f, .5f, -0.5f}, {.7f, .3f, .95f}},
      {{-.5f, -.5f, -0.5f}, {.6f, .2f, .9f}},
      {{.5f, -.5f, -0.5f}, {.7f, .3f, .95f}},
      {{.5f, .5f, -0.5f}, {.8f, .4f, 1.0f}},

  };
  for (auto &v : vertices) {
    v.position += offset;
  }
  return std::make_unique<SuzumeModel>(device, vertices);
}

void FirstApp::loadGameObjects() {
  std::shared_ptr<SuzumeModel> suzumeModel =
      createCubeModel(suzumeDevice, {.0f, .0f, .0f});
  auto cube = SuzumeGameObject::createGameObject();
  cube.model = suzumeModel;
  cube.transform.translation = {.0f, .0f, .5f};
  cube.transform.scale = {.5f, .5f, .5f};
  gameObjects.push_back(std::move(cube));
}

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

  for (auto &obj : gameObjects) {
    obj.transform.rotation.y =
        glm::mod(obj.transform.rotation.y + 0.01f, glm::two_pi<float>());
    obj.transform.rotation.x =
        glm::mod(obj.transform.rotation.x + 0.005f, glm::two_pi<float>());

    SuzumePushConstantData push{};
    push.color = obj.color;
    push.transform = obj.transform.mat4();

    vkCmdPushConstants(commandBuffer, pipelineLayout,
                       VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                       0, sizeof(SuzumePushConstantData), &push);
    obj.model->bind(commandBuffer);
    obj.model->draw(commandBuffer);
  }
}

} // namespace Suzume
