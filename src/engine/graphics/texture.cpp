#include "texture.hpp"

#include "../debug/logger.hpp"

#include <boost/dll/runtime_symbol_info.hpp>
#include <map>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace open_pokemon_tcg::engine::graphics;

static std::map<std::string, GLuint> _cache;

Texture::Texture(std::string img_path) {
  if (_cache.count(img_path)) {
    _texture = _cache[img_path];
    return;
  }

  stbi_set_flip_vertically_on_load