#include "player.hpp"

#include "../../engine/debug/logger.hpp"
#include "playmat.hpp"

#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>

#include <algorithm>
#include <iterator>

using namespace open_pokemon_tcg::game::view;

Player::Player(const model::Player &model, const IPlaymat &playmat, IPlaymat::Side playmat_side)
  : _model(mo