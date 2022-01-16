#include "transform.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>

using namespace open_pokemon_tcg::engine::geometry;

Transform::Transform() : Transform(glm::vec3(0.0f)) {}
Transform::Transform(glm::vec3 position) : Transform(position, glm::vec3(0.0f)) {}
Transform::Transform(glm::vec3 position, glm::vec3 rotation) : Transform(position, rotation, glm::vec3(1.0f)) {}
Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
  this->position = position;
  this->rotation = rotation;
  this->scale = scale;
}

Transform::~Transform() {}

void Transform::set_rotation(float yaw,