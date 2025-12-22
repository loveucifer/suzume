#pragma once
#include "SuzumeDevice.hpp"
#include "SuzumeSwapChain.hpp"
#include "SuzumeWindow.hpp"
#include "vulkan/vulkan_core.h"

// random include libraries that are standard :3
#include <cassert>
#include <memory>
#include <vector>

namespace Suzume {
class SuzumeRenderer {

public:
  SuzumeRenderer(SuzumeWindow &window, SuzumeDevice &device);
  ~SuzumeRenderer();
  SuzumeRenderer(const SuzumeRenderer &) = delete;
  SuzumeRenderer &operator=(const SuzumeRenderer &) = delete;
  VkRenderPass getSwapChainRenderPass() const {
    return suzumeSwapChain
        ->getRenderPass(); // small s lol for swap cahin idk where i fumbled
  }

  bool isFrameInProgress() const { return isFrameStarted; }
  VkCommandBuffer getCurrentCommandBuffer() const {
    assert(isFrameStarted &&
           "Cannot get command buffer when frame is not in progress");
    return commandBuffers[currentFrameIndex];
  }

  int getFrameIndex() const {
    assert(isFrameStarted && "Cannot get frame index when frame not in progress");
    return currentFrameIndex;
  }

  VkCommandBuffer beginFrame();
  void endFrame();
  void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
  void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

private:
  void createCommandBuffers();
  void freeCommandBuffers();
  void drawFrame();
  void recreateSwapChain();

  SuzumeWindow &suzumeWindow;
  SuzumeDevice &suzumeDevice;
  std::shared_ptr<SuzumeSwapChain> suzumeSwapChain;
  // order matters note to self
  std::vector<VkCommandBuffer> commandBuffers;

  uint32_t currentImageIndex;
  int currentFrameIndex{0};
  bool isFrameStarted;
};
} // namespace Suzume
