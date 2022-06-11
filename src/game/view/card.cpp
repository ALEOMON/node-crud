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
  glm::vec3 botleft  = rect.botleft();
  glm::vec3 botright = rect.botright();
  glm::vec3 topleft  = rect.topleft();
  glm::vec3 topright = rect.topright();

  this->vao = create_vao({
    //	 X      Y     Z
    botleft.x , botleft.y , botleft.z,
    botright.x, botright.y, botright.z,
    topright.x, topright.y, topright.z,
    topleft.x , topleft.y , topleft.z,
  }, {
      0.0f, 0.0f, // (u,v) for v0
      1.0f, 0.0f, // (u,v) for v1
      1.0f, 1.0f, // (u,v) for v2
      0.0f, 1.0f, // (u,v) for v3
  });
}

Card::~Card() {}

void Card::render(const glm::mat4 &view_projection_matrix, engine::graphics::Shader *shader) {
  shader->use();

  glm::mat4 front_matrix = this->transform.matrix();
  glm::mat4 modelViewProjectionMatrix = view_projection_matrix * front_matrix;
  shader->set_uniform("modelViewProjectionMatrix", &modelViewProjectionMatrix[0].x);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, this->front_texture.id());
  glBindVertexArr