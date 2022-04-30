#include "energy_removal.hpp"

#include "../../../engine/debug/logger.hpp"

using namespace open_pokemon_tcg::game::model;

EnergyRemoval::EnergyRemoval(CardEffectTarget pokemon_scope)
  : _pokemon_scope(pokemon_scope) {}

EnergyRemoval::~EnergyRemoval() = default;

void EnergyRemoval::activate([[maybe_unused]] Player &self, Player &opponent, std::vector<std::reference_wrapper<const ICard>> targets) {
  const PokemonCard* pokemon_target = dynamic_cast<const PokemonCard*>(