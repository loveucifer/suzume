#define GLFW_INCLUDE_VULKAN
#include <SuzumeWindow.hpp>
#include <vulkan/vulkan.h>

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

void SuzumeWindow::createWindowSurface(VkInstance instance,
                                       VkSurfaceKHR *surface) {
  if (glfwCreateWindowSurface(instance, window, nullptr, surface) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create window surface!");
  }
}
