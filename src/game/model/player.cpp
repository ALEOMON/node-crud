
#include "player.hpp"

#include "../../engine/debug/logger.hpp"

#include <boost/lexical_cast.hpp>

using namespace open_pokemon_tcg::game::model;

Player::Player(std::unique_ptr<Deck>& deck, Playmat& playmat, std::string name)
  : _deck(std::move(deck)),
    _playmat(playmat),
    _name(name) {

  _playmat.deck_pile = new DeckPile(*_deck);
  _playmat.deck_pile->shuffle();

  _playmat.discard_pile = new DiscardPile();
  _playmat.bench = new Bench();

  std::array<std::reference_wrapper<ICard>, 6> prize_cards = {
    _playmat.deck_pile->pop(),
    _playmat.deck_pile->pop(),
    _playmat.deck_pile->pop(),
    _playmat.deck_pile->pop(),
    _playmat.deck_pile->pop(),
    _playmat.deck_pile->pop(),
  };

  _playmat.prize_card_pool = new PrizeCardPool(prize_cards);
}

Player::~Player() {}

void Player::draw(unsigned int amount) {
  if (_playmat.deck_pile->size() < amount) {
    _on_lose();
    return;
  }

  for (unsigned int i = 0; i < amount; i++)
    _hand.add(_playmat.deck_pile->pop());
}

void Player::mill(unsigned int amount) {
  if (_playmat.deck_pile->size() < amount) {
    _on_lose();
    return;
  }

  for (unsigned int i = 0; i < amount; i++)
    _playmat.discard_pile->push(_playmat.deck_pile->pop());
}

void Player::discard(const ICard& card) {
  _playmat.discard_pile->push(_hand.take(_hand.find(card)));
}

void Player::place_active_pokemon(unsigned int hand_index) {
  if (hand_index >= _hand.size())
    LOG_ERROR("Hand size is " + std::to_string(_hand.size()) + ", so could not use card at index " + std::to_string(hand_index));

  if (_playmat.active_pokemon != nullptr)
    LOG_ERROR("Can not place active pokemon when there already is one.");

  _playmat.active_pokemon = (PokemonCard*) &_hand.cards()[hand_index];
  _on_update_active(_playmat.active_pokemon);
  _hand.remove(hand_index);
}

void Player::switch_active(const PokemonCard& pokemon_card) {
  unsigned int bench_index = _playmat.bench->find(pokemon_card);
  PokemonCard& card = _playmat.bench->take(bench_index);
  _playmat.bench->place(*_playmat.active_pokemon, bench_index);