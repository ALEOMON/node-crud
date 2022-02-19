#include "rectangle.hpp"

#include "../debug/logger.hpp"

#include <glm/vec3.hpp>

#include <vector>

using namespace open_pokemon_tcg::engine::graphics;

Rectangle::Rectangle(geometry::Rectangle rectangle, const Texture *texture) : _texture(texture){
  _shader = std::make_unique<Shader>("texture.vert", "texture.frag");

  glm::vec3 botleft  = rectangle.botleft();
  glm::vec3 botright = rectangle.botright();
  glm::vec3 topleft  = rectangle.topleft();
  glm::vec3 topright = rectangle.topright();

  std::vector<float> positions = {
    //	 X      Y     Z
      botleft.x , botleft.y , botleft.z,
      botright.x, botright.y, botright.z,
      topright.x, topright.y, topright.z,
      topleft.x , topleft.y , topleft.z,
  };

  unsigned int pos_buffer;
  glGenBuffers(1, &pos_buffer); // Gen buffer object and store buffer id
  glBindB