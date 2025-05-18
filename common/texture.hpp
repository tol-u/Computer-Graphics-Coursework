#pragma once

#include <GL/glew.h> // For GLuint
#include <string> // Often useful, though not strictly for this declaration

// Function declaration for loading a texture
unsigned int loadTexture(const char *path);

// Note: The actual STB_IMAGE_IMPLEMENTATION and the definition of loadTexture
// will be moved to common/texture.cpp to prevent duplicate symbols.
