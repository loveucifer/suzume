#pragma once
#include "SuzumeDevice.hpp"
#include "vulkan/vulkan_core.h"
#include <SuzumePipeline.hpp>
#include <SuzumeSwapChain.hpp>
#include <SuzumeWindow.hpp>

// random include libraries that are standard :3
#include <memory>
#include <vector>

namespace Suzume {
class FirstApp {

public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  FirstApp();
  ~FirstApp();
  FirstApp(const FirstApp &) = delete;
  FirstApp &operator=(const FirstApp &) = delete;

  void run();

private:
  void createPipelineLayout();
  void createPipeline();
  void createCommandBuffers();
  void drawFrame();

  SuzumeWindow window{WIDTH, HEIGHT, "Vulkan"};
  SuzumeDevice device{window};
  SuzumeSwapChain swapChain{device, window.getExtent()};
  // order matters note to self
  std::unique_ptr<SuzumePipeline> suzumePipeline;
  VkPipelineLayout pipelineLayout;
  std::vector<VkCommandBuffer> commandBuffers;
};
} // namespace Suzume
