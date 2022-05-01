#pragma once

#include "card_effect.hpp"

namespace open_pokemon_tcg::game::model {

  class Heal : public ICardEffect {
  public:
    Heal(CardEffectTarget scope, unsigned int heal_amount);
    ~Heal();

    // Mutators
    void activate(Player &self, Player &opponent, std::vector<std::reference_wrapper<const ICard>> targets) o