#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace Suzume {
class SuzumeWindow {
public:
  SuzumeWindow(int w, int h, const std::string &title);
  ~SuzumeWindow(); // cleanup window later when we want to destroy it

  SuzumeWindow(const SuzumeWindow &) = delete;
  SuzumeWindow &operator=(const SuzumeWindow &) = delete;

  bool shouldClose() {
    return (glfwWindowShouldClose(window));
  } // used elsewhere read code idiot

  void pollEvents();
  bool shouldClose() const;

private:
  void initwindow();
  const int width;
  const int height;
  std::string windowName;

  GLFWwindow *window;
};
} // namespace Suzume
