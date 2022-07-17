#pragma once

#include "card.hpp"
#include "../model/card/pokemon.hpp"

namespace open_pokemon_tcg::game::view {
  class PokemonCard {
  public:
    PokemonCard(game::model::PokemonCard &model, engine::geometry::Transform transform);
    ~PokemonCard();

    // Mutators
    void render(const glm::mat4 &view_projection_matrix, engine::graphics::Shader *shader);

    // Accessors
    glm::mat4 model_matrix() const;
    engine::geometry::Inte