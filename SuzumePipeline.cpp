#include "SuzumePipeline.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

SuzumePipeline::SuzumePipeline(const std::string &vertFilePath,
                               const std::string &fragFilePath) {
  createGraphicsPipeline(vertFilePath, fragFilePath);
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

 void SuzumePipeline::createGraphicsPipeline(const std::string &vertFilePath,
                                             const std::string &fragFilePath) {
   auto vertCode = readFile(vertFilePath);
   auto fragCode = readFile(fragFilePath);

   std::cout << "vertex shader code size: " << vertCode.size() << std::endl;
   std::cout << "fragment shader code size: " << fragCode.size() << std::endl;
 }
