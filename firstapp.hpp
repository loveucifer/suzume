#pragma once
#include "SuzumeDevice.hpp"
#include <SuzumePipeline.hpp>
#include <SuzumeWindow.hpp>
namespace Suzume {
class firstapp {
public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  firstapp();
  void run();

private:
  SuzumeWindow window;
  SuzumeDevice device{window};
  SuzumePipeline pipeline{
      device, "shaders/SuzumeShader.vert.spv",
      "shaders/SuzumeShader.frag.spv",
      SuzumePipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
};
} // namespace Suzume
