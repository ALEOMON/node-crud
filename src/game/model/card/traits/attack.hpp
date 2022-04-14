#pragma once

#include "energy.hpp"
#include "../../effects/attack_effect.hpp"

#include <memory>
#include <string>

namespace open_pokemon_tcg::game::model {

  class PokemonCard;

  class Attack {
  public:
    Attack(std::string name, unsigned int damage, EnergyAmount cost);
    Attack(std::string name, unsigned int da