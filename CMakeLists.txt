cmake_minimum_required(VERSION 3.0)
project(Coursework)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Add all external libraries by including the CMakeLists.txt in the external directory
# This will build GLFW, GLEW, etc., and set up their include paths.
add_subdirectory(external)

# Add our executable using coursework.cpp
add_executable(Coursework src/coursework.cpp)

# Explicitly tell Coursework where to find various headers
# Paths are relative to this CMakeLists.txt file (project root)
target_include_directories(Coursework PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw-3.1.2/include  # For GLFW/glfw3.h
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glm-0.9.7.1        # For glm/glm.hpp etc. (GLM is header-only)
    # GLEW headers should be handled by linking GLEW_1130 which has INTERFACE includes
)

# Link Coursework against OpenGL, GLEW, GLFW, and necessary macOS frameworks
# GLEW_1130 and glfw are targets created by external/CMakeLists.txt
target_link_libraries(Coursework PRIVATE
    ${OPENGL_LIBRARIES}
    GLEW_1130                # Target from external/CMakeLists.txt
    glfw                     # Target from external/CMakeLists.txt (via external/glfw-3.1.2)
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreFoundation"
    "-framework CoreGraphics"
    "-framework CoreVideo"
)

# If you need to specify include directories for GLFW headers explicitly (e.g., if not in default paths):
# target_include_directories(Coursework PRIVATE /path/to/glfw/include) # Adjust path if needed 