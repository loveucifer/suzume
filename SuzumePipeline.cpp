#include "SuzumePipeline.hpp"
#include "vulkan/vulkan_core.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

SuzumePipeline::SuzumePipeline(Suzume::SuzumeDevice &device,
                               const std::string &vertFilePath,
                               const std::string &fragFilePath,
                               const PipelineConfigInfo &configinfo)
    : device(device) {
  createGraphicsPipeline(vertFilePath, fragFilePath, configinfo);
}

std::vector<char> SuzumePipeline::readFile(const std::string &filePath) {
  std::ifstream file(filePath, std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    throw std::runtime_error("failed to open file panic!");
  }

  size_t fileSize = static_cast<size_t>(file.tellg());
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();

  return buffer;
}

SuzumePipeline::~SuzumePipeline() {}

void SuzumePipeline::createGraphicsPipeline(
    const std::string &vertFilePath, const std::string &fragFilePath,
    const PipelineConfigInfo &configinfo) {
  auto vertCode = readFile(vertFilePath);
  auto fragCode = readFile(fragFilePath);

  std::cout << "vertex shader code size: " << vertCode.size() << std::endl;
  std::cout << "fragment shader code size: " << fragCode.size() << std::endl;
}

void SuzumePipeline::createShaderModule(const std::vector<char> &code,
                                        VkShaderModule *shaderModule) {
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

  if (vkCreateShaderModule(device.device(), &createInfo, nullptr,
                           shaderModule) != VK_SUCCESS) {
    throw std::runtime_error("failed to create shader module nooo !");
  }
}

PipelineConfigInfo SuzumePipeline::defaultPipelineConfigInfo(uint32_t width,
                                                             uint32_t height) {
  PipelineConfigInfo configInfo{};

  configInfo.inputAssemblyInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

  return configInfo;
}
