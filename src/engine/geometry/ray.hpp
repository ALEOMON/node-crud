#pragma once

#include <glm/vec3.hpp>

namespace open_pokemon_tcg::engine::geometry {

  struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;
  };

}
