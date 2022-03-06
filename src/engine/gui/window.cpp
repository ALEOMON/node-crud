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

  GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
  if (!window) {
    glfwTerminate();
    LOG_CRITICAL("Failed to create glfw window.");
  }

  glfwMakeContextCurrent(window);
  glfwSetWindowUserPointer(window, reinterpret_cast<void *>(this));

  glfwSetKeyCallback(window, [](GLFWwindow* window, [[maybe_unused]] int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
    Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    for (std::function<void(GLFWwindow*, int)> c : w->on_key_callbacks)
      c(window, action);
  });

  glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, [[maybe_unused]] int mods) {
    Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    for (std::function<void(GLFWwindow*, int, int)> c : w->on_mouse_click_callbacks)
      c(window, button, action);
  });

  glfwSetCursorPosCallback(window, [](GLFWwindow* 