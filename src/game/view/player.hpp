#pragma once

#include "bench.hpp"
#include "card.hpp"
#include "pokemon_card.hpp"
#include "deck_pile.hpp"
#include "discard_pile.hpp"
#include "hand.hpp"
#include "playmat.hpp"
#include "prize_card_pool.hpp"

#include "../model/player.hpp"

namespace open_pokemon_tcg::game::view {

  class Player {
  public:
    Player(const model::Player &model, const IPlaymat &playmat, IPlaymat::Side playmat_side);
