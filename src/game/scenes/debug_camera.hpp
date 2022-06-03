#pragma once

#include "../../engine/graphics/camera.hpp"
#include "../../engine/gui/window.hpp"

namespace open_pokemon_tcg::game::scenes {

  class DebugCamera {
  public:
    DebugCamera(engine::gui::Window* window);
    DebugCamera(engine::gui::Window* window, engine::geometry::Transform transform);
    ~DebugCamera();

    // Mutators
    void set_transform(engine::geometry::Transform transform);

    // Accessors
    engine::geometry::Transform transform() const { return this->camera.transform; };
    glm::mat4 view_matrix(