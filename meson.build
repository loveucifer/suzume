project('vulkan_app', 'cpp',
  default_options: ['cpp_std=c++20']
)

vulkan_dep = dependency('vulkan')
glfw_dep = dependency('glfw3')
glm_dep = dependency('glm')

executable(
  'vulkan_app',
  'main.cpp',
  'FirstApp.cpp',
  'SuzumeWindow.cpp',
  'SuzumeDevice.cpp',
  'SuzumePipeline.cpp',
  'SuzumeSwapChain.cpp',
  'SuzumeModel.cpp',
  dependencies: [vulkan_dep, glfw_dep, glm_dep],
)
