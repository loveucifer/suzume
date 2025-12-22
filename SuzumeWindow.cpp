#include "SuzumeWindow.hpp"
#include <stdexcept>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

namespace Suzume {

void SuzumeWindow::framebufferResizeCallback(GLFWwindow *window, int width,
                                             int height) {
  auto suzumeWindow =
      reinterpret_cast<SuzumeWindow *>(glfwGetWindowUserPointer(window));
  suzumeWindow->framebufferResized = true;
  suzumeWindow->width = width;
  suzumeWindow->height = height;
}

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
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  window =
      glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
  glfwSetWindowUserPointer(window, this);
  glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void SuzumeWindow::createWindowSurface(VkInstance instance,
                                       VkSurfaceKHR *surface) {
  if (glfwCreateWindowSurface(instance, window, nullptr, surface) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create window surface!");
  }
}

} // namespace Suzume