#include "rectangle.hpp"

#include "../debug/logger.hpp"

#include <glm/vec3.hpp>

#include <vector>

using namespace open_pokemon_tcg::engine::graphics;

Rectangle::Rectangle(geometry::Rectangle rectangle, const Texture *texture) : _texture(texture){
  _shader = std::make_unique<Shader>("texture.vert", "texture.frag");

  glm::vec3 botleft  = rectangle.botleft();
  glm::vec3 b