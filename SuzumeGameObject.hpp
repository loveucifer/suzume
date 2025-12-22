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
  glm::vec2 scale{1.0f, 1.0f};
  float rotation{0.0f};

  glm::mat2 mat2() {
    const float s = glm::sin(rotation);
    const float c = glm::cos(rotation);
    glm::mat2 rotMatrix = {{c, s}, {-s, c}};
    glm::mat2 scaleMat = {{scale.x, 0.0f},
                          {0.0f, scale.y}}; // takes columns not rows
    return rotMatrix *
           scaleMat; // basic highschoool maths i worked it out on paper lol
  }
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
