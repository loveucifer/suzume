#pragma once
#include "SuzumeDevice.hpp"
#include "SuzumeGameObject.hpp"
#include "SuzumePipeline.hpp"
#include "vulkan/vulkan_core.h"

// random include libraries that are standard :3
#include <memory>
#include <vector>

namespace Suzume {
class SuzumeRenderSystem {

public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  SuzumeRenderSystem(SuzumeDevice &deviceRef, VkRenderPass renderPass);
  ~SuzumeRenderSystem();
  SuzumeRenderSystem(const SuzumeRenderSystem &) = delete;
  SuzumeRenderSystem &operator=(const SuzumeRenderSystem &) = delete;
  void renderGameObjects(VkCommandBuffer commandBuffer,
                         std::vector<SuzumeGameObject> &gameObjects);

private:
  void createPipelineLayout();
  void createPipeline(VkRenderPass renderPass);

  SuzumeDevice &suzumeDevice;

  std::unique_ptr<SuzumePipeline> suzumePipeline;
  VkPipelineLayout pipelineLayout;
};
} // namespace Suzume
