#pragma once

#include "card.hpp"
#include "../model/deck_pile.hpp"

#include "../../engine/geometry/transform.hpp"
#include "../../engine/graphics/shader.hpp"

#include <glm/mat4x4.hpp>

#include <vector>

namespace open_pokemon_tcg::game::view {

  class DeckPile {
  public:
    DeckPile(const model::DeckPile &model, engine::geometry::Transform transform)