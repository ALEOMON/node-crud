#include "card.hpp"

#include "../../engine/debug/logger.hpp"
#include "../../engine/debug/debug_drawer.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>

using namespace open_pokemon_tcg;
using namespace open_pokemon_tcg::game::view;

Card::Card(const game::model::ICard &model, engine::geometry::Transform transform)
  : _model(model),
    transform(transform),
    front_texture(engine::graphics::Texture("cache/cards/img/" + _model.id() + ".png")),
    back_texture(engine::graphics::Texture("img/cardback.png")) {

  engine::geometry::Rectangle rect(engine::geometry::Transform(), this->width, this->height);
  glm::vec3 botleft  = rect.botlef