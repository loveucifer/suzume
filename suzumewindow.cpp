#include "GLFW/glfw3.h"
#include <SuzumeWindow.hpp>

namespace Suzume {
SuzumeWindow::SuzumeWindow(int w, int h, const std::string &title)
    : width(w), height(h), windowName(title), window(nullptr) {
  initwindow();
}
SuzumeWindow::~SuzumeWindow() {
  glfwDestroyWindow(window);
  glfwTerminate();
}
void SuzumeWindow::initwindow() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  window =
      glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
}

void SuzumeWindow::pollEvents() { glfwPollEvents(); }

bool SuzumeWindow::shouldClose() const { return glfwWindowShouldClose(window); }
} // namespace Suzume
