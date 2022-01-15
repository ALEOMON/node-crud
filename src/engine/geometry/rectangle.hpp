#pragma once

#include "transform.hpp"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace open_pokemon_tcg::engine::geometry {

  class Rectangle {
  public:
    Rectangle();
    Rectangle(Transform transform, float width = 1.0f, float height = 1.0f);

    // Accessors
    Transform transform() const;
    float width() const;
    float height() const;

    glm::vec3 topleft() const;
    glm::vec3 topright() const;
    glm::vec3 botleft() const;
    glm::vec3 botr