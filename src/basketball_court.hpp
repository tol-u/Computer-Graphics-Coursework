#ifndef BASKETBALL_COURT_HPP
#define BASKETBALL_COURT_HPP

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "../common/model.hpp" // Corrected path

class BasketballCourt {
public:
    BasketballCourt(const std::string& modelPath, const std::string& texturePath);
    ~BasketballCourt();

    void draw(GLuint shaderID, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
    void update(float deltaTime); // Even if static, good for consistency

private:
    Model* model;
    GLuint textureID;
    glm::vec3 position;
    glm::vec3 rotation; // Euler angles in degrees
    glm::vec3 scale;
};

#endif 