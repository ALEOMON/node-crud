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
    positions.emplace_back(line.A.z);
    positions.emplace_back(line.B.x);
    positions.emplace_back(line.B.y);
    positions.emplace_back(line.B.z);
  }

  unsigned int pos_buffer;
  glGenBuffers(1, &pos_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, pos_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * positions.size(), positions.data(), GL_STATIC_DRAW);

  std::vector<float> colors;
  for (auto &line : this->line_queue) {
    for (int i = 0; i < 2; i++) {
      colors.emplace_back(line.color.x);
 