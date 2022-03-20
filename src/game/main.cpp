#include "scenes/duel.hpp"
// #include "scenes/debug/card_transform.hpp"
// #include "scenes/debug/deck_loading.hpp"
// #include "scenes/debug/model.hpp"
// #include "scenes/debug/playmat_slots.hpp"

#include "../engine/debug/logger.hpp"
#include "../engine/gui/window.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <string>


using namespace open_pokemon_tcg;

void gui(engine::scene::IScene* scene) {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  scene->gui();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main() {
  srand(time(NULL));

  engine::debug::Logger::set_profile(engine::debug::Logger::Profile::DEBUG);
  LOG_INFO("Progra