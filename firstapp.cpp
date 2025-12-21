#include <firstapp.hpp>

namespace Suzume {
firstapp::firstapp() : window(WIDTH, HEIGHT, "Suzume testing") {}

void firstapp::run() {
  while (!this->window.shouldClose()) {
    glfwPollEvents();
  }
}
} // namespace Suzume
