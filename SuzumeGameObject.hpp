#pragma once

#include "SuzumeModel.hpp"
#include "glm/fwd.hpp"
#include "vulkan/vulkan_core.h"

// std shit
#include <memory>

namespace Suzume {

struct Transform2dComponent {
  glm::vec2
      translations{}; // movement of traversal mostly ig like position offset
  glm::mat2 mat2() { return glm::mat2{1.0f}; }
};

class SuzumeGameObject {
public:
  using id_t = unsigned int;

  static SuzumeGameObject create() {
    static id_t currentId = 0;
    return SuzumeGameObject(currentId++);
  }

  SuzumeGameObject(const SuzumeGameObject &) = delete;
  SuzumeGameObject &operator=(const SuzumeGameObject &) = delete;
  SuzumeGameObject(SuzumeGameObject &&) = default;
  SuzumeGameObject &operator=(SuzumeGameObject &&) = default;

  id_t getId() const { return id; }

  std::shared_ptr<SuzumeModel> model{};
  glm::vec3 color{};
  Transform2dComponent transform2d{};

private:
  SuzumeGameObject(id_t objId) : id{objId} {}
  id_t id;
};

}; // namespace Suzume
