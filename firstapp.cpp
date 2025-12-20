#include <firstapp.hpp>

namespace suzume {
firstapp::firstapp() : suzumewindow(WIDTH, HEIGHT, "suzume testing") {}

void firstapp::run() {
  while (!suzumewindow.shouldClose()) {
    suzumewindow.pollEvents();
  }
}
} // namespace suzume
