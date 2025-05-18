#pragma once

#include <GL/glew.h>
// GLFW is not strictly needed for just the declaration, 
// but often included with glew. Can be removed if causing issues.
#include <GLFW/glfw3.h>

// Forward declarations or minimal includes needed for the function signature
// (In this case, none beyond GLuint from glew.h)

// Declaration of LoadShaders
GLuint LoadShaders(const char *vertex_file_path, 
                   const char *fragment_file_path);
