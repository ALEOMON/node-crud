
#pragma once

#include "../../view/card.hpp"
#include "../debug_camera.hpp"

#include "../../../engine/debug/logger.hpp"
#include "../../../engine/graphics/texture.hpp"
#include "../../../engine/gui/window.hpp"
#include "../../../engine/scene/scene.hpp"

#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <imgui.h>

namespace open_pokemon_tcg::game::scenes {

class CardTransform : public engine::scene::IScene {
  public:
    CardTransform(engine::gui::Window* window);
    ~CardTransform();