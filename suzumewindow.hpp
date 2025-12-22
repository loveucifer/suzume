#pragma once
#include "vulkan/vulkan_core.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <vulkan/vulkan.h>

namespace Suzume {

class SuzumeWindow {
public:
  SuzumeWindow(int w, int h, const std::string &title);
  ~SuzumeWindow(); // cleanup window later when we want to destroy it

  SuzumeWindow(const SuzumeWindow &) = delete;
  SuzumeWindow &operator=(const SuzumeWindow &) = delete;

  bool shouldClose() { return (glfwWindowShouldClose(window)); }

  VkExtent2D getExtent() {
    return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
  }

  bool wasWindowResized() { return framebufferResized; }
  void resetWindowResizedFlag() { framebufferResized = false; }

  void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

private:
  static void framebufferResizeCallback(GLFWwindow *window, int width, int height);
  void initwindow();
  int width;
  int height;
  bool framebufferResized =
      false; // did this for readability ignore if u are looking a it later

  std::string windowName;

  GLFWwindow *window;
};

} // namespace Suzume
