#include "basketball_player.hpp"
#include "../common/texture.hpp" // Correct path to texture.hpp
#include "../common/maths.hpp" // Include MyMaths
#include <GLFW/glfw3.h> // For glfwGetKey
// #include <glm/gtc/matrix_transform.hpp> // No longer directly needed for these ops

BasketballPlayer::BasketballPlayer(const std::string& modelPath, const std::string& diffuseTexturePath, const std::string& normalTexturePath) {
    model = new Model(modelPath.c_str());
    this->diffuseTextureID = loadTexture(diffuseTexturePath.c_str());
    this->normalTextureID = loadTexture(normalTexturePath.c_str()); // Load normal map

    position = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation = glm::vec3(0.0f, 0.0f, 0.0f); // Euler angles in degrees
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

BasketballPlayer::~BasketballPlayer() {
    delete model;
    glDeleteTextures(1, &diffuseTextureID);
    glDeleteTextures(1, &normalTextureID); // Delete normal map texture
}

void BasketballPlayer::update(float deltaTime) {
    // rotation.y += 10.0f * deltaTime; // Keep or remove existing auto-rotation as desired
    // if (rotation.y > 360.0f) rotation.y -= 360.0f;
}

void BasketballPlayer::processPlayerKeyboardInput(GLFWwindow* window, float deltaTime) {
    float moveSpeed = 2.0f * deltaTime; // Units per second
    float rotSpeed = 45.0f * deltaTime; // Degrees per second

    // Movement (e.g., Arrow keys)
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        position.z -= moveSpeed; // Move forward along -Z
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        position.z += moveSpeed; // Move backward along +Z
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        position.x -= moveSpeed; // Move left along -X
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        position.x += moveSpeed; // Move right along +X
    }

    // Optional: Rotation (e.g., J and L for Y-axis rotation)
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        rotation.y += rotSpeed;
        if (rotation.y > 360.0f) rotation.y -= 360.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        rotation.y -= rotSpeed;
        if (rotation.y < 0.0f) rotation.y += 360.0f;
    }
    
    // Optional: Up/Down movement (e.g., R and F keys)
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        position.y += moveSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        position.y -= moveSpeed;
    }
}

void BasketballPlayer::draw(GLuint shaderID, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    // Apply transformations using MyMaths functions
    glm::mat4 scaleMatrix = MyMaths::createScaleMatrix(scale);
    glm::mat4 rotXMatrix = MyMaths::createRotationMatrixX(rotation.x);
    glm::mat4 rotYMatrix = MyMaths::createRotationMatrixY(rotation.y);
    glm::mat4 rotZMatrix = MyMaths::createRotationMatrixZ(rotation.z);
    glm::mat4 transMatrix = MyMaths::createTranslationMatrix(position);

    // Order: Scale -> Rotate -> Translate (T * Rz * Ry * Rx * S)
    modelMatrix = transMatrix * rotZMatrix * rotYMatrix * rotXMatrix * scaleMatrix;

    glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;

    glUniformMatrix4fv(glGetUniformLocation(shaderID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, &modelMatrix[0][0]);
    
    // Bind textures
    glActiveTexture(GL_TEXTURE0); // Diffuse map to texture unit 0
    glBindTexture(GL_TEXTURE_2D, this->diffuseTextureID);
    // glUniform1i(glGetUniformLocation(shaderID, "texture_diffuse"), 0); // Sampler set in coursework.cpp

    glActiveTexture(GL_TEXTURE1); // Normal map to texture unit 1
    glBindTexture(GL_TEXTURE_2D, this->normalTextureID);
    // glUniform1i(glGetUniformLocation(shaderID, "texture_normal"), 1); // Sampler set in coursework.cpp

    model->draw(shaderID);
} 