#include "deck_pile.hpp"

#include "../../engine/debug/logger.hpp"

#include <glm/ext/scalar_constants.hpp>

using namespace open_pokemon_tcg::game::view;

DeckPile::DeckPile(const model::DeckPile &model, engine::geometry::Transform transform)
  : _model(model),
    transform(transform) {

  for (auto card : _model.cards())
    _cards.push_back(Card(card, engine