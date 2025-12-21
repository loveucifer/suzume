#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "SuzumeDevice.hpp"
#include "vulkan/vulkan_core.h"
#include <cstdint>
#include <glm/glm.hpp>

namespace Suzume {
class SuzumeModel {

public:
  struct Vertex {
    glm::vec2 position;
    glm::vec3 color;

    static std::vector<VkVertexInputBindingDescription>
    getBindingDescriptions();
    static std::vector<VkVertexInputAttributeDescription>
    getAttributeDescriptions();
  };

  SuzumeModel(SuzumeDevice &device, const std::vector<Vertex> &vertices);
  ~SuzumeModel();

  SuzumeModel(const SuzumeModel &) = delete;
  SuzumeModel &operator=(const SuzumeModel &) = delete;

  void bind(VkCommandBuffer commandBuffer);
  void draw(VkCommandBuffer commandBuffer);

private:
  void createVertexBuffer(const std::vector<Vertex> &vertices);
  SuzumeDevice &device;
  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;
  uint32_t vertexCount;
};
} // namespace Suzume
