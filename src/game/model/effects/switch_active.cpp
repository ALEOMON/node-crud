#include "switch_active.hpp"

#include "../../../engine/debug/logger.hpp"

using namespace open_pokemon_tcg::game::model;

SwitchActive::SwitchActive(CardEffectTarget scope)
  : _scope(scope) {}

SwitchActive::~SwitchActive() = default;

void SwitchActive::activate(Player &self, Player &opponent, std::v