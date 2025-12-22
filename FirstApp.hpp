#pragma once
#include "SuzumeDevice.hpp"
#include "SuzumeGameObject.hpp"
#include "SuzumePipeline.hpp"
#include "SuzumeRenderer.hpp"
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
  void loadGameObjects();
  void createPipelineLayout();
  void createPipeline();
  void renderGameObjects(VkCommandBuffer commandBuffer);

  SuzumeWindow suzumeWindow{WIDTH, HEIGHT, "Suzume"};
  SuzumeDevice suzumeDevice{suzumeWindow};
  SuzumeRenderer suzumeRenderer{suzumeWindow, suzumeDevice};
  std::unique_ptr<SuzumePipeline> suzumePipeline;
  VkPipelineLayout pipelineLayout;
  std::vector<SuzumeGameObject> gameObjects;
};
} // namespace Suzume
