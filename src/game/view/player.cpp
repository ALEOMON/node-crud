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
    this->hand = new Hand(_model.hand(), engine::geometry::Transform(glm::vec3(0.0f, 0.5f, -4.75f),
                                                 glm::vec3(0.5f * glm::half_pi<float>(), glm::pi<float>(), 0.0f)));
  else
    this->hand = new Hand(_model.hand(), engine::geometry::Transform(glm::vec3(0.0f, 0.5f, 4.75f),
                                                 glm::vec3(0.5f * glm::half_pi<float>(), 0.0f, 0.0f)));

  std::array<engine::geometry::Rectangle, 6> prize_slots = this->playmat->prize_slots(this->playmat_side);
  std::arra