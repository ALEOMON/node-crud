#pragma once

#include "../../view/card.hpp"
#include "../debug_camera.hpp"
#include "../../data/pokemon_tcg_api.hpp"

#include "../../../engine/debug/logger.hpp"
#include "../../../engine/graphics/texture.hpp"
#include "../../../engine/gui/window.hpp"
#include "../../../engine/scene/scene.hpp"

#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <imgui.h>

namespace open_pokemon_tcg::game::scenes {

class DeckLoading : public engine::scene::IScene {
  public:
    DeckLoading(engine::gui::Window* window);
    ~DeckLoading();

    void update() override;
    void render() override;
    void gui() override;

  private:
    PokemonTcgApi* api;
    DebugCamera camera;
    std::vector<Card> cards;
    engine::graphics::Shader *shader;

    // GUI options
    char deck_id[100] = { 0 };
    bool one_card_type_per_row = false;

    void load_deck(std::string deck_id);
  };

  DeckLoading::DeckLoading(engine::gui::Window* window) :
    camera(DebugCamera(window,
                       engine::geometry::Transform(glm::vec3(0.0f, 2.0f, 0.0f),
                                 glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f)))) {

    this->shader = new engine::graphics::Shader("simple.vert", "simple.frag");

    this->api = new PokemonTcgApi();
  }
  DeckLoading::~DeckLoading() {}

  void DeckLoading::update() {}
  void