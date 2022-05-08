#pragma once

#include "player.hpp"
#include "playmat.hpp"
#include "effects/card_effect.hpp"

#include <memory>
#include <string>
#include <array>

namespace open_pokemon_tcg::game::model {

  class SandboxGame {
  public:
    SandboxGame(std::array<std::unique_ptr<Deck>, 2>& player_decks, std::array<std::string, 2> player_names);
    ~SandboxGame();

    friend class Game;

    // Mutators
    void place_on_active_slot_from_hand(const ICard& card);
    void place_on_bench_from_hand(const ICard& card);
    void place_on_bench_from_hand(const ICard& card, unsigned int slot_index)