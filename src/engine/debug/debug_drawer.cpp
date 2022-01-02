#include "debug_drawer.hpp"

#include "logger.hpp"

using namespace open_pokemon_tcg::engine::debug;

DebugDrawer::DebugDrawer() : shader(graphics::Shader("color.vert", "color.frag")) {}
DebugDrawer::~DebugDrawer() = default;

void DebugDrawer::render(const glm::mat4 &view_projection_matrix) {

  this->shader.use();

  std::vector<float> positions;
  for (auto &line : this->line_queue) {
    positions.emplace_back(line.A.x);
    positions.emplace_back(line.A.y);
    positions.emp