#include "hand.hpp"

#include "../../engine/debug/logger.hpp"

#include <algorithm>

using namespace open_pokemon_tcg::game::view;


Hand::Hand(const model::Hand& model, engine::geometry::Transform transform