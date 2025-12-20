#pragma once
#include <suzumewindow.hpp>

namespace suzume {
class firstapp {
public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  firstapp();
  void run();

private:
  suzumewindow suzumewindow;
};
} // namespace suzume
