#pragma once

#include "deck.hpp"
#include "card/card.hpp"

#include "../../engine/event/event.hpp"

#include <functional>
#include <vector>

namespace open_pokemon_tcg::game::model {

  class DeckPile {
  public:
    DeckPile();
    DeckPile(Deck& deck);
    ~DeckPile();

    // Mutators
    ICard& pop();
    void shuffle();

   