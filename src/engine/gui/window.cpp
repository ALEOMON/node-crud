#include "window.hpp"
#include "../debug/logger.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <stdexcept>

using namespace open_pokemon_tcg::engine::gui;

Window::Window() : Window(1920, 1080, "OpenPokemonTCG") {}

Window::Window(int width, int height, const char* title) {
  if (!glfwInit()) {
    LOG_CRITICAL("Failed to init glfw.");
  }

  GLFWwindow* window = glfwCreateWindow