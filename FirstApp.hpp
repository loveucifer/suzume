#pragma once
#include "SuzumeDevice.hpp"
#include "SuzumeGameObject.hpp"
#include "SuzumePipeline.hpp"
#include "SuzumeSwapChain.hpp"
#include "SuzumeWindow.hpp"
#include "vulkan/vulkan_core.h"

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
  void loadGameObjects(); // migrated to gameobjects from models , couldve
                          // commented out but we have git diff
  void createPipelineLayout();
  void createPipeline();
  void createCommandBuffers();
  void freeCommandBuffers();
  void drawFrame();
  void recreateSwapChain();
  void recordCommandBuffers(int imageIndex);
  void renderGameObjects(VkCommandBuffer commandBuffer);

  SuzumeWindow suzumeWindow{WIDTH, HEIGHT, "Suzume"};
  SuzumeDevice suzumeDevice{suzumeWindow};
  std::unique_ptr<SuzumeSwapChain> suzumeSwapChain;
  // order matters note to self
  std::unique_ptr<SuzumePipeline> suzumePipeline;
  VkPipelineLayout pipelineLayout;
  std::vector<VkCommandBuffer> commandBuffers;
  std::vector<SuzumeGameObject> gameObjects;
};
} // namespace Suzume
