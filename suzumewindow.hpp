#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <string>

class SuzumeWindow {
public:
  SuzumeWindow(int w, int h, const std::string &title);
  ~SuzumeWindow(); // cleanup window later when we want to destroy it

  SuzumeWindow(const SuzumeWindow &) = delete;
  SuzumeWindow &operator=(const SuzumeWindow &) = delete;

  bool shouldClose() {
    return (glfwWindowShouldClose(window));
  } // used elsewhere read code idiot

  void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

private:
  void initwindow();
  const int width;
  const int height;
  std::string windowName;

  GLFWwindow *window;
};
