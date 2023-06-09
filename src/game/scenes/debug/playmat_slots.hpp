
#pragma once

#include "../../view/card.hpp"
#include "../debug_camera.hpp"
#include "../../playmats/black_playmat.hpp"
#include "../../playmats/green_playmat.hpp"

#include "../../../engine/debug/logger.hpp"
#include "../../../engine/debug/debug_drawer.hpp"
#include "../../../engine/graphics/texture.hpp"
#include "../../../engine/gui/window.hpp"
#include "../../../engine/scene/scene.hpp"
#include "../../../engine/graphics/rectangle.hpp"
#include "playmat.hpp"

#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <imgui.h>

namespace open_pokemon_tcg::game::scenes {

class PlaymatSlots : public engine::scene::IScene {
  public:
    PlaymatSlots(engine::gui::Window* window);
    ~PlaymatSlots();

    void update() override;
    void render() override;
    void gui() override;

  private:
    DebugCamera camera;
    engine::graphics::Shader *shader;
    open_pokemon_tcg::game::view::IPlaymat *playmat;

    std::vector<Card> cards;
    Card debug_card;

    // GUI options
    bool show_cards = true;
    bool show_debug_card = false;
    glm::vec3 debug_card_pos = glm::vec3(0.0f, 0.02f, 0.0f);
    glm::vec3 debug_card_rot = glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f);
  };

  PlaymatSlots::PlaymatSlots(engine::gui::Window* window) :
    camera(DebugCamera(window,
                       engine::geometry::Transform(glm::vec3(0.0f, 10.0f, 0.0f),
                                 glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f)))),
    debug_card(Card(engine::geometry::Transform(), engine::graphics::Texture("img/cardback.png").id())) {

    this->shader = new engine::graphics::Shader("simple.vert", "simple.frag");
    // this->playmat = new playmats::BlackPlaymat();
    this->playmat = new playmats::GreenPlaymat();

    // Player 1
    engine::graphics::Texture texture1("cache/cards/img/base1-8.png");
    this->cards.push_back(Card(playmat->active_slot(open_pokemon_tcg::game::view::IPlaymat::Side::PLAYER1).transform(), texture1.id()));
    this->cards.push_back(Card(playmat->supporter_slot(open_pokemon_tcg::game::view::IPlaymat::Side::PLAYER1).transform(), texture1.id()));
    this->cards.push_back(Card(playmat->stadium_slot(open_pokemon_tcg::game::view::IPlaymat::Side::PLAYER1).transform(), texture1.id()));
    for (int i = 0; i < 5; i++)
      this->cards.push_back(Card(playmat->bench_slots(open_pokemon_tcg::game::view::IPlaymat::Side::PLAYER1)[i].transform(), texture1.id()));
    for (int i = 0; i < 6; i++)
      this->cards.push_back(Card(playmat->prize_slots(open_pokemon_tcg::game::view::IPlaymat::Side::PLAYER1)[i].transform(), texture1.id()));
    this->cards.push_back(Card(playmat->deck_slot(open_pokemon_tcg::game::view::IPlaymat::Side::PLAYER1).transform(), texture1.id()));
    this->cards.push_back(Card(playmat->discard_slot(open_pokemon_tcg::game::view::IPlaymat::Side::PLAYER1).transform(), texture1.id()));

    // Player 2
    engine::graphics::Texture texture2("cache/cards/img/base1-24.png");
    this->cards.push_back(Card(playmat->active_slot(open_pokemon_tcg::game::view::IPlaymat::Side::PLAYER2).transform(), texture2.id()));
    this->cards.push_back(Card(playmat->supporter_slot(open_pokemon_tcg::game::view::IPlaymat::Side::PLAYER2).transform(), texture2.id()));
    this->cards.push_back(Card(playmat->stadium_slot(open_pokemon_tcg::game::view::IPlaymat::Side::PLAYER2).transform(), texture2.id()));
    for (int i = 0; i < 5; i++)
      this->cards.push_back(Card(playmat->bench_slots(open_pokemon_tcg::game::view::IPlaymat::Side::PLAYER2)[i].transform(), texture2.id()));
    for (int i = 0; i < 6; i++)
      this->cards.push_back(Card(playmat->prize_slots(open_pokemon_tcg::game::view::IPlaymat::Side::PLAYER2)[i].transform(), texture2.id()));
    this->cards.push_back(Card(playmat->deck_slot(open_pokemon_tcg::game::view::IPlaymat::Side::PLAYER2).transform(), texture2.id()));
    this->cards.push_back(Card(playmat->discard_slot(open_pokemon_tcg::game::view::IPlaymat::Side::PLAYER2).transform(), texture2.id()));
  }
  PlaymatSlots::~PlaymatSlots() {}

  void PlaymatSlots::update() {
    this->debug_card.transform.position = debug_card_pos;
    this->debug_card.transform.rotation = debug_card_rot;
  }

  void PlaymatSlots::render() {
    glm::mat4 viewMatrix = this->camera.view_matrix();
    glm::mat4 projectionMatrix = this->camera.projection_matrix();
    glm::mat4 view_projection_matrix = projectionMatrix * viewMatrix;

    this->shader->use();

    if (show_debug_card)
      this->debug_card.render(view_projection_matrix, this->shader);

    if (show_cards)
      for (Card &c : cards)
        c.render(view_projection_matrix, this->shader);

    this->playmat->render(view_projection_matrix, this->shader);
  }

  void PlaymatSlots::gui() {
    ImGui::Begin("Playmat");
    ImGui::Checkbox("Show cards", &show_cards);
    ImGui::End();
    ImGui::Begin("Debug Card");
    ImGui::Checkbox("Show", &show_debug_card);
    ImGui::DragFloat3("position", (float*)&debug_card_pos, -10.0f      , 10.0f);
    ImGui::DragFloat3("rotation", (float*)&debug_card_rot, -2*glm::pi<float>(), 2*glm::pi<float>());
    ImGui::End();
  }
}