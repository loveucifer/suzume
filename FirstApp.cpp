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
  recreateSwapChain();
  createPipelineLayout();
  createPipeline();
  createCommandBuffers();
}

FirstApp::~FirstApp() {
  vkDestroyPipelineLayout(suzumeDevice.device(), pipelineLayout, nullptr);
}

void FirstApp::run() {
  while (!suzumeWindow.shouldClose()) {
    glfwPollEvents();
    drawFrame();
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

  assert(suzumeSwapChain != nullptr &&
         "cannot create pipeline layout before swap chain");

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
  pipelineConfig.renderPass = suzumeSwapChain->getRenderPass();
  pipelineConfig.pipelineLayout = pipelineLayout;
  suzumePipeline = std::make_unique<SuzumePipeline>(
      suzumeDevice, "shaders/SuzumeShader.vert.spv",
      "shaders/SuzumeShader.frag.spv", pipelineConfig);
}

void FirstApp::recreateSwapChain() {
  auto extent = suzumeWindow.getExtent();
  while (extent.width == 0 || extent.height == 0) {
    extent = suzumeWindow.getExtent();
    glfwWaitEvents(); // for minimization shit
  }
  vkDeviceWaitIdle(suzumeDevice.device());

  if (suzumeSwapChain == nullptr) {
    suzumeSwapChain = std::make_unique<SuzumeSwapChain>(suzumeDevice, extent);
  } else {
    std::shared_ptr<SuzumeSwapChain> oldSwapChain = std::move(suzumeSwapChain);
    suzumeSwapChain =
        std::make_unique<SuzumeSwapChain>(suzumeDevice, extent, oldSwapChain);

    if (suzumeSwapChain->imageCount() != commandBuffers.size()) {
      freeCommandBuffers();
      createCommandBuffers();
    }
  }

  // lfuture otpimization : check if render passes are compatible and if so  do
  // absolute zilch
}

void FirstApp::createCommandBuffers() {
  commandBuffers.resize(suzumeSwapChain->imageCount());

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = suzumeDevice.getCommandPool();
  allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

  if (vkAllocateCommandBuffers(suzumeDevice.device(), &allocInfo,
                               commandBuffers.data()) != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate command buffers!");
  }
}

void FirstApp::freeCommandBuffers() {
  vkFreeCommandBuffers(
      suzumeDevice.device(), suzumeDevice.getCommandPool(),
      static_cast<uint32_t>(
          commandBuffers.size()), // dont forget this is not fucking float u
                                  // broke it lots of times get fucking sleep
      commandBuffers.data());
  commandBuffers.clear();
}

void FirstApp::recordCommandBuffers(int imageIndex) {

  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to begin recording command buffer!");
  }

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = suzumeSwapChain->getRenderPass();
  renderPassInfo.framebuffer = suzumeSwapChain->getFrameBuffer(imageIndex);

  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = suzumeSwapChain->getSwapChainExtent();

  std::array<VkClearValue, 2> clearValues{};
  clearValues[0].color = {{0.01f, 0.01f, 0.01f, 1.0f}};
  clearValues[1].depthStencil = {1.0f, 0};
  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();

  vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo,
                       VK_SUBPASS_CONTENTS_INLINE);

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width =
      static_cast<float>(suzumeSwapChain->getSwapChainExtent().width);
  viewport.height =
      static_cast<float>(suzumeSwapChain->getSwapChainExtent().height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  VkRect2D scissor{{0, 0}, suzumeSwapChain->getSwapChainExtent()};
  vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
  vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

  renderGameObjects(commandBuffers[imageIndex]);

  vkCmdEndRenderPass(commandBuffers[imageIndex]);
  if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
    throw std::runtime_error("failed to record command buffer!");
  }
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

void FirstApp::drawFrame() {
  uint32_t imageIndex;
  auto result = suzumeSwapChain->acquireNextImage(&imageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    recreateSwapChain();
    createPipeline();
    return;
  }

  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("failed to acquire swap chain image!");
  }

  recordCommandBuffers(imageIndex);
  result = suzumeSwapChain->submitCommandBuffers(&commandBuffers[imageIndex],
                                                 &imageIndex);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      suzumeWindow.wasWindowResized()) {
    suzumeWindow.resetWindowResizedFlag();
    recreateSwapChain();
    createPipeline();
    return;
  }
  if (result != VK_SUCCESS) {
    throw std::runtime_error("failed to present swap chain image!");
  }
}

} // namespace Suzume
