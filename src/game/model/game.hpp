#pragma once

#include "sandbox_game.hpp"

namespace open_pokemon_tcg::game::model {

  class Game {
  public:
    Game(std::array<std::unique_ptr<Deck>, 2>& player_decks, std::array<std::string, 2> player_names);
    ~Game();

    // Mutators
    void place_on_active_slot_from_hand(const ICard&