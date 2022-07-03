#include "player.hpp"

#include "../../engine/debug/logger.hpp"
#include "playmat.hpp"

#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>

#include <algorithm>
#include <iterator>

using namespace open_pokemon_tcg::game::view;

Player::Player(const model::Player &model, const IPlaymat &playmat, IPlaymat::Side playmat_side)
  : _model(model),
    playmat(&playmat),
    playmat_side(playmat_side) {

  _shader = new engine::graphics::Shader("simple.vert", "simple.frag");

  this->deck_pile = new DeckPile(*_model.playmat().deck_pile, playmat.deck_slot(this->playmat_side).transform());

  this->discard_pile = new DiscardPile(*_model.playmat().discard_pile, playmat.discard_slot(this->playmat_side).transform());

  if (playmat_side == open_pokemon_tcg::game::view::IPlaymat::Side::PLAYER1)
    this->hand = new Hand(_mo