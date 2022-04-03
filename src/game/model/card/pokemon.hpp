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

    bool is_gx;
    bool is_alolan;
    EvolutionStage stage;
    CardName evolves_from;
    int hp;
    EnergyType energy_type;
    EnergyAmount retreat_cost;
    std::vector<Attack*> attacks;
  };

  class PokemonCard : public ICard {
  public:
    PokemonCard(PokemonCardData data);
    ~PokemonCard();

    // Mutators
    void take_damage(unsigned int amount, EnergyType type);
    void heal(unsigned int amount);
    void attach_energy(BasicEnergy& energy_card);

    BasicEnergy& detach_en