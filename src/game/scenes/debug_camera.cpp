#include "debug_camera.hpp"

#include "../../engine/debug/logger.hpp"

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>

#include <algorithm>

using namespace open_pokemon_tcg::game::scenes;

DebugCamera::DebugCamera(engine::gui::Window* window) : camera(engine::graphics::Camera()), window(window){}
DebugCamera::DebugCamera(engine::gui::Window* window, engine::geometry::Transform transform) : camera(engine::graphics::Camera(transform)), window(window) {
  this->window->add_on_key_callback(std::bind(&DebugCamera::on_key, this, std::placeholders::_1));
  this->window->add_on_mouse_move_callback(std::bind(&DebugCamera::on_cursor, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  