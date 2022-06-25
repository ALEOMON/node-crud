#include "hand.hpp"

#include "../../engine/debug/logger.hpp"

#include <algorithm>

using namespace open_pokemon_tcg::game::view;


Hand::Hand(const model::Hand& model, engine::geometry::Transform transform)
  : _model(model),
    _transform(transform) {

  for (auto card : _model.cards())
    _cards.push_back(new Card(card, engine::geometry::Transform()));
  update();

  _model.listen_on_add([this](model::ICard& card) {
    _cards.push_back(new Card(card, engine::geometry::Transform()));
    update();
  });

  _model.listen_on_remove([this](int index) {
    _cards.erase(_cards.begin() + index);
    update();
  });
}

Hand::~Hand() {}

// Mutators
void Hand::update() {
  int count = _cards.size();
