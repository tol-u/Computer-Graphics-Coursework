#ifndef RIM_HPP
#define RIM_HPP

#include <string> // For std::string
#include <GL/glew.h> // For GLuint
#include <glm/glm.hpp> // For glm types
#include "../common/model.hpp" // For Model class

class Rim {
public:
    Rim(const std::string& modelPath, const std::string& texturePath);
    ~Rim();

    void draw(GLuint shaderID, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
    void update(float deltaTime);

private:
    Model* model;
    GLuint textureID;
    glm::vec3 position;
    glm::vec3 rotation; // Euler angles in degrees
    glm::vec3 scale;
};

#endif // RIM_HPP 