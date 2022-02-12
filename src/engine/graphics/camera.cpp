#include "camera.hpp"
#include "../debug/logger.hpp"

#include <algorithm>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

using namespace open_pokemon_tcg::engine::graphics;

// FIXME
bool firstMouse = true;
double lastX;
double lastY;
double yaw = 0.0f;
double pitch = 0.0f;

Camera::Camera() : Camera(geometry::Transform()){}
Camera::Camera(const geometry::Transform& transform) : transform(transform){
  yaw = this->transform.yaw();
  pitch = this->transform.pitch();
}

Camera::~Camera() = default;

void Camera::move(Direction move_dir) {
  glm::vec3 displacement_dir;
  switch (move_dir) {
  case Direction::FORWARD:
    displacement_dir = this->transform.forward();
    break;
  case Direction::BACKWARD:
    displacement_dir = -this->transform.forward();
    break;
  case Direction::RIGHT:
    displacement_dir = this->transform.right();
    break;
  case Direction::LEFT:
    displacement_dir = -this->transform.right();
    break;
  case Direction::UP:
    displacement_dir = this->transform.up();
    break;
  case Direction::DOWN:
    displacement_dir = -this->transform.up();
    break;
  default:
    LOG_ERROR("Direction type not supported: " + std::to_string(move_dir));
  }
  this->transform.position += this->movement_speed * displacement_dir;
}

// void Camera::look_at(glm::vec3 target) {
//   this->orientation = Orientation(target - this->pos);
// }

void Camera::set_zoom(float zoom_level) {
  // FIXME: name constant
  this->perspective.fov = 45.0f - zoom_level;
}

void Camera::lookat_mouse(float mouse_xpos, float mouse_ypos) {
  if (firstMou