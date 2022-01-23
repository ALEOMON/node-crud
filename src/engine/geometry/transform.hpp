#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace open_pokemon_tcg::engine::geometry {

  class Transform {
  public:
    Transform();
    Transform(glm::vec3 position);
    Transform(glm::vec3 position, glm::vec3 rotation);
    Transform(glm::vec3 po