#include "basketball_court.hpp"
#include "../common/texture.hpp" // Assuming this is the correct path for loadTexture
#include "../common/maths.hpp" // Include MyMaths
// #include <glm/gtc/matrix_transform.hpp> // No longer directly needed

BasketballCourt::BasketballCourt(const std::string& modelPath, const std::string& texturePath) {
    model = new Model(modelPath.c_str());
    
    // Generate a dummy texture for the court (light brown/wooden)
    GLubyte courtColor[3] = {200, 160, 100}; // Light brown wooden color
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, courtColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Try to load the actual texture, but don't worry if it fails
    GLuint loadedTexID = loadTexture(texturePath.c_str());
    if (loadedTexID != 0) {
        glDeleteTextures(1, &textureID);
        textureID = loadedTexID;
    }

    // Initial position, rotation, and scale for the court (typically static)
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    // Adjust scale based on the model size - may need tweaking
    scale = glm::vec3(0.2f, 0.2f, 0.2f); // Increased scale to match the basketball size
}

BasketballCourt::~BasketballCourt() {
    delete model;
    glDeleteTextures(1, &textureID);
}

void BasketballCourt::update(float deltaTime) {
    // Court is static, so this function can be empty for now.
    // (void)deltaTime; // To suppress unused parameter warning if your compiler is strict
}

void BasketballCourt::draw(GLuint shaderID, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
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
    // Note: View and Projection matrices are set in the main loop in coursework.cpp before this draw call.
    // Light and object color uniforms are also set in the main loop.

    // Set texture to unit 0 and make sure binding is correct
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->textureID);
    glUniform1i(glGetUniformLocation(shaderID, "texture_diffuse"), 0);
    
    // Provide a dummy texture for the normal map to avoid errors
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, this->textureID); // Just reuse the diffuse texture
    glUniform1i(glGetUniformLocation(shaderID, "texture_normal"), 1);

    model->draw(shaderID);
} 