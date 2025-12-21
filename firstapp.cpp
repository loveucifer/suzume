#include <firstapp.hpp>

namespace Suzume {
firstapp::firstapp() : SuzumeWindow(WIDTH, HEIGHT, "Suzume testing") {}

void firstapp::run() {
  while (!this->SuzumeWindow.shouldClose()) {
    glfwPollEvents();
  }
}
} // namespace Suzume
