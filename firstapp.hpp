#pragma once
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
  SuzumeWindow SuzumeWindow;
   SuzumePipeline SuzumePipeline{"shaders/SuzumeShader.vert.spv",
                                 "shaders/SuzumeShader.frag.spv"};
};
} // namespace Suzume
