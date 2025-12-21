project('vulkan_app', 'cpp',
  default_options: ['cpp_std=c++20']
)

vulkan_dep = dependency('vulkan')
glfw_dep = dependency('glfw3')

executable(
  'vulkan_app',
  'main.cpp',
  'firstapp.cpp',
  'SuzumeWindow.cpp',
  'SuzumePipeline.cpp',
  dependencies: [vulkan_dep, glfw_dep],
)
