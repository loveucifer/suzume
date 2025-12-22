#pragma once
#include "SuzumeDevice.hpp"
#include "vulkan/vulkan_core.h"
#include <string>
#include <vector>

namespace Suzume {
struct PipelineConfigInfo {

  VkPipelineViewportStateCreateInfo viewportStateInfo;
  VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
  VkPipelineRasterizationStateCreateInfo rasterizationInfo;
  VkPipelineMultisampleStateCreateInfo multisampleInfo;
  VkPipelineColorBlendAttachmentState colorBlendAttachment;
  VkPipelineColorBlendStateCreateInfo colorBlendInfo;
  VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
  VkRenderPass renderPass = nullptr;
  std::vector<VkDynamicState> dynamicStateEnables;
  VkPipelineDynamicStateCreateInfo dynamicStateInfo;
  VkPipelineLayout pipelineLayout = nullptr;
  uint32_t subpass = 0;
};

class SuzumePipeline {
public:
  SuzumePipeline(SuzumeDevice &device, const std::string &vertFilePath,
                 const std::string &fragFilePath,
                 const PipelineConfigInfo &configInfo);
  ~SuzumePipeline();

  SuzumePipeline(const SuzumePipeline &) = delete;
  SuzumePipeline &operator=(const SuzumePipeline &) = delete;

  void bind(VkCommandBuffer commandBuffer);

  static void defaultPipelineConfigInfo(PipelineConfigInfo &configInfo);

private:
  static std::vector<char> readFile(const std::string &filePath);

  void createGraphicsPipeline(const std::string &vertFilePath,
                              const std::string &fragFilePath,
                              const PipelineConfigInfo &configInfo);

  void createShaderModule(
      const std::vector<char> &code,
      VkShaderModule *shaderModule); // haha pointer to a pointer lmaoo wtf

  SuzumeDevice &device;
  VkPipeline graphicsPipeline;
  VkShaderModule vertShaderModule;
  VkShaderModule fragShaderModule;
};
} // namespace Suzume
