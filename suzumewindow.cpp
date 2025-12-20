#include "GLFW/glfw3.h"
#include <suzumewindow.hpp>

namespace suzume {
suzumewindow::suzumewindow(int w, int h, const std::string &title)
    : width(w), height(h), windowName(title), window(nullptr) {
  initwindow();
}
suzumewindow::~suzumewindow() {
  glfwDestroyWindow(window);
  glfwTerminate();
}
void suzumewindow::initwindow() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  window =
      glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
}

void suzumewindow::pollEvents() {
  glfwPollEvents();
}

bool suzumewindow::shouldClose() const {
  return glfwWindowShouldClose(window);
}
} // namespace suzume
