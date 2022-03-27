#pragma once

#include "card.hpp"
#include "basic_energy.hpp"
#include "traits/attack.hpp"
#include "traits/energy.hpp"
#include "traits/pokemon_evolution_stage.hpp"

#include "../../../engine/event/event.hpp"

#include <functional>
#include <vector>

namespace open_pokemon_tcg::game::model {

  struct PokemonCardData {
    CardId id;
    CardName name;

  