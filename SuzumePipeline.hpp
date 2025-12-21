#pragma once
#include "SuzumeDevice.hpp"
#include "SuzumePipeline.hpp"
#include "vulkan/vulkan_core.h"
#include <string>
#include <vector>

struct PipelineConfigInfo {
  VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
  VkPipelineRasterizationStateCreateInfo rasterizer;
  VkPipelineMultisampleStateCreateInfo multisampling;
  VkPipelineColorBlendAttachmentState colorBlendAttachment;
  VkPipelineColorBlendStateCreateInfo colorBlending;
  VkPipelineViewportStateCreateInfo viewportState;
  VkPipelineDepthStencilStateCreateInfo depthStencil;
  VkPipelineDynamicStateCreateInfo dynamicState;
};

class SuzumePipeline {
public:
  SuzumePipeline(Suzume::SuzumeDevice &device, const std::string &vertFilePath,
                 const std::string &fragFilePath,
                 const PipelineConfigInfo &configInfo);
  ~SuzumePipeline();

  SuzumePipeline(const SuzumePipeline &) = delete;
  void operator=(const SuzumePipeline &) = delete;

  static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width,
                                                      uint32_t height);

private:
  static std::vector<char> readFile(const std::string &filePath);

  void createGraphicsPipeline(const std::string &vertFilePath,
                              const std::string &fragFilePath,
                              const PipelineConfigInfo &configInfo);

  void createShaderModule(
      const std::vector<char> &code,
      VkShaderModule *shaderModule); // haha pointer to a pointer lmaoo wtf

  Suzume::SuzumeDevice &device;
  VkPipeline graphicsPipeline;
  VkShaderModule vertShaderModule;
  VkShaderModule fragShaderModule;
}; // namespace Suzume
