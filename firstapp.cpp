#include <firstapp.hpp>

namespace Suzume {
firstapp::firstapp() : SuzumeWindow(WIDTH, HEIGHT, "Suzume testing") {}

void firstapp::run() {
  while (!SuzumeWindow.shouldClose()) {
    SuzumeWindow.pollEvents();
  }
}
} // namespace Suzume
