#ifndef BASKETBALL_PLAYER_HPP
#define BASKETBALL_PLAYER_HPP

#include <string> // For std::string
#include <GL/glew.h> // For GLuint
#include <glm/glm.hpp> // For glm types
struct GLFWwindow; // Forward declaration for GLFWwindow as including glfw3.h in headers can be problematic
                 // Alternatively, include <GLFW/glfw3.h> if it's managed well.
                 // For simplicity here, let's include it, assuming it's okay in this project structure.
#include <GLFW/glfw3.h> 
#include "../common/model.hpp" // For Model class

class BasketballPlayer {
public:
    BasketballPlayer(const std::string& modelPath, const std::string& diffuseTexturePath, const std::string& normalTexturePath);
    ~BasketballPlayer();

    void draw(GLuint shaderID, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
    void update(float deltaTime);
    void processPlayerKeyboardInput(GLFWwindow* window, float deltaTime);

private:
    Model* model;
    GLuint diffuseTextureID;
    GLuint normalTextureID;
    glm::vec3 position;
    glm::vec3 rotation; // Euler angles in degrees
    glm::vec3 scale;
};

#endif // BASKETBALL_PLAYER_HPP