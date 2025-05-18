#ifndef BASKETBALL_HPP
#define BASKETBALL_HPP

#include <string> // For std::string
#include <GL/glew.h> // For GLuint
#include <glm/glm.hpp> // For glm types
#include <glm/gtc/quaternion.hpp> // For quaternions
#include "../common/model.hpp" // For Model class

class Basketball {
public:
    Basketball(const std::string& modelPath, const std::string& texturePath);
    ~Basketball();

    void draw(GLuint shaderID, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
    void update(float deltaTime);
    void resetBall(); // New method to reset the ball

private:
    Model* model;
    GLuint textureID;
    glm::vec3 position;
    // glm::vec3 rotation; // Euler angles in degrees - Replaced by quaternion
    glm::quat orientation; // Quaternion for orientation
    glm::vec3 scale;
};

#endif 