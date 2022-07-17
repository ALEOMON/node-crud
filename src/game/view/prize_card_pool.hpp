#pragma once

#include "card.hpp"
#include "../model/prize_card_pool.hpp"
#include "../../engine/geometry/transform.hpp"

#include <array>

namespace open_pokemon_tcg::game::view {

  class PrizeCardPool {
  public:
    PrizeCardPool(const model::PrizeCardPool &mo