#pragma once
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
};
} // namespace Suzume
