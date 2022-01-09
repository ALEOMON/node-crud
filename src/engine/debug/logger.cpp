#include "logger.hpp"

#include <string>
#include <glad/glad.h>

using namespace open_pokemon_tcg::engine::debug;

bool checkGLError(const char* file, int line) {
	bool wasError = false;
	for (GLenum glErr = glGetError(); glErr != GL_NO_ER