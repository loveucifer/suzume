#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace suzume {
class suzumewindow {
public:
  suzumewindow(int w, int h, const std::string &title);
  ~suzumewindow(); // cleanup window later when we want to destroy it

  suzumewindow(const suzumewindow &) = delete;
  suzumewindow &operator=(const suzumewindow &) = delete;

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
} // namespace suzume
