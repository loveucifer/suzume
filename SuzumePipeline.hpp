#pragma once
#include <string>
#include <vector>

class SuzumePipeline {
public:
  SuzumePipeline(const std::string &vertFilePath,
                 const std::string &fragFilePath);
  ~SuzumePipeline();

private:
  static std::vector<char> readFile(const std::string &filePath);

  void createGraphicsPipeline(const std::string &vertFilePath,
                               const std::string &fragFilePath);
};
