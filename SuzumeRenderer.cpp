#include "SuzumeRenderer.hpp"

// std
#include <array>
#include <cassert>
#include <stdexcept>

namespace Suzume {

SuzumeRenderer::SuzumeRenderer(SuzumeWindow &window, SuzumeDevice &device)
    : suzumeWindow{window}, suzumeDevice{device} {
  recreateSwapChain();
  createCommandBuffers();
}

SuzumeRenderer::~SuzumeRenderer() { freeCommandBuffers(); }

void SuzumeRenderer::recreateSwapChain() {
  auto extent = suzumeWindow.getExtent();
  while (extent.width == 0 || extent.height == 0) {
    extent = suzumeWindow.getExtent();
    glfwWaitEvents();
  }
  vkDeviceWaitIdle(suzumeDevice.device());

  if (suzumeSwapChain == nullptr) {
    suzumeSwapChain = std::make_shared<SuzumeSwapChain>(suzumeDevice, extent);
  } else {
    std::shared_ptr<SuzumeSwapChain> oldSwapChain = suzumeSwapChain;
    suzumeSwapChain =
        std::make_shared<SuzumeSwapChain>(suzumeDevice, extent, oldSwapChain);

    if (!oldSwapChain->compareSwapFormats(*suzumeSwapChain.get())) {
      throw std::runtime_error(
          "Swap chain image(or depth) format has changed!");
    }
  }
}

void SuzumeRenderer::createCommandBuffers() {
  commandBuffers.resize(SuzumeSwapChain::MAX_FRAMES_IN_FLIGHT);

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

void SuzumeRenderer::freeCommandBuffers() {
  vkFreeCommandBuffers(suzumeDevice.device(), suzumeDevice.getCommandPool(),
                       static_cast<uint32_t>(commandBuffers.size()),
                       commandBuffers.data());
  commandBuffers.clear();
}

VkCommandBuffer SuzumeRenderer::beginFrame() {
  assert(!isFrameStarted && "Can't call beginFrame while already in progress");

  auto result = suzumeSwapChain->acquireNextImage(&currentImageIndex);
  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    recreateSwapChain();
    return nullptr;
  }

  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("failed to acquire swap chain image!");
  }

  isFrameStarted = true;

  auto commandBuffer = getCurrentCommandBuffer();
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
    throw std::runtime_error("failed to begin recording command buffer!");
  }
  return commandBuffer;
}

void SuzumeRenderer::endFrame() {
  assert(isFrameStarted &&
         "Can't call endFrame while frame is not in progress");
  auto commandBuffer = getCurrentCommandBuffer();
  if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
    throw std::runtime_error("failed to record command buffer!");
  }

  auto result =
      suzumeSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      suzumeWindow.wasWindowResized()) {
    suzumeWindow.resetWindowResizedFlag();
    recreateSwapChain();
  } else if (result != VK_SUCCESS) {
    throw std::runtime_error("failed to present swap chain image!");
  }

  isFrameStarted = false;
  currentFrameIndex =
      (currentFrameIndex + 1) % SuzumeSwapChain::MAX_FRAMES_IN_FLIGHT;
}

void SuzumeRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
  assert(isFrameStarted &&
         "Can't call beginSwapChainRenderPass if frame is not in progress");
  assert(commandBuffer == getCurrentCommandBuffer() &&
         "Can't begin render pass on command buffer from a different frame");

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = suzumeSwapChain->getRenderPass();
  renderPassInfo.framebuffer =
      suzumeSwapChain->getFrameBuffer(currentImageIndex);

  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = suzumeSwapChain->getSwapChainExtent();

  std::array<VkClearValue, 2> clearValues{};
  clearValues[0].color = {{0.01f, 0.01f, 0.01f, 1.0f}};
  clearValues[1].depthStencil = {1.0f, 0};
  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();

  vkCmdBeginRenderPass(commandBuffer, &renderPassInfo,
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
  vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
  vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void SuzumeRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) {
  assert(isFrameStarted &&
         "Can't call endSwapChainRenderPass if frame is not in progress");
  assert(commandBuffer == getCurrentCommandBuffer() &&
         "Can't end render pass on command buffer from a different frame");
  vkCmdEndRenderPass(commandBuffer);
}

} // namespace Suzume
