#include "SuzumeRenderSystem.hpp"

#include "SuzumeDevice.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/constants.hpp"
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>
#include <vulkan/vulkan_core.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <cassert>

namespace Suzume {

struct SuzumePushConstantData {
  glm::mat4 transform{
      1.0f}; // default @ identity matrix       // 8 bytes at offset 0
  alignas(16) glm::vec3
      color; //  12 bytes at offset 16 (vec3 requires 16-byte alignment in GLSL)
};

SuzumeRenderSystem::SuzumeRenderSystem(SuzumeDevice &device,
                                       VkRenderPass renderPass)
    : suzumeDevice{device} {

  createPipelineLayout();
  createPipeline(renderPass);
}

SuzumeRenderSystem::~SuzumeRenderSystem() {
  vkDestroyPipelineLayout(suzumeDevice.device(), pipelineLayout, nullptr);
}

void SuzumeRenderSystem::createPipelineLayout() {

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

void SuzumeRenderSystem::createPipeline(VkRenderPass renderPass) {
  PipelineConfigInfo pipelineConfig{};
  SuzumePipeline::defaultPipelineConfigInfo(pipelineConfig);
  pipelineConfig.renderPass = renderPass;
  pipelineConfig.pipelineLayout = pipelineLayout;
  suzumePipeline = std::make_unique<SuzumePipeline>(
      suzumeDevice, "shaders/SuzumeShader.vert.spv",
      "shaders/SuzumeShader.frag.spv", pipelineConfig);
}

void SuzumeRenderSystem::renderGameObjects(
    VkCommandBuffer commandBuffer, std::vector<SuzumeGameObject> &gameObjects) {
  suzumePipeline->bind(commandBuffer);
  // updater for testing shit
  int i = 0;
  for (auto &obj : gameObjects) {
    i += 1;
    obj.transform.rotation.y =
        glm::mod<float>(obj.transform.rotation.y + 0.001f * i,
                        2.f * glm::pi<float>()); // rotate veritcally
    obj.transform.rotation.x =
        glm::mod<float>(obj.transform.rotation.x + 0.0005f * i,
                        2.f * glm::pi<float>()); // rotate horizontally
  }
  // renderer

  for (auto &obj : gameObjects) {
    SuzumePushConstantData push{};
    push.color = obj.color;
    push.transform = obj.transform.mat4();

    vkCmdPushConstants(commandBuffer, pipelineLayout,
                       VK_SHADER_STAGE_VERTEX_BIT |
                           VK_SHADER_STAGE_FRAGMENT_BIT,
                       0, sizeof(SuzumePushConstantData), &push);
    obj.model->bind(commandBuffer);
    obj.model->draw(commandBuffer);
  }
}

} // namespace Suzume
