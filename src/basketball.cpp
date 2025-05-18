#include "basketball.hpp"
#include "../common/texture.hpp"
#include "../common/maths.hpp"
#include <glm/gtc/quaternion.hpp> // For glm::quat, glm::angleAxis
#include <glm/gtx/quaternion.hpp> // For glm::mat4_cast
#include <iostream> // For debugging output

Basketball::Basketball(const std::string& modelPath, const std::string& texturePath) {
    model = new Model(modelPath.c_str());
    
    // Generate a extremely bright red texture for maximum visibility
    GLubyte redColor[3] = {255, 0, 0}; // Bright red
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, redColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Try to load the actual texture, but use our bright red texture instead
    GLuint loadedTexID = loadTexture(texturePath.c_str());
    if (loadedTexID != 0) {
        glDeleteTextures(1, &textureID);
        textureID = loadedTexID;
    }
    
    // Position the ball very close to the camera for maximum visibility
    position = glm::vec3(0.0f, 0.0f, 5.0f); 
    orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); 
    // Make the ball enormous for better visibility
    scale = glm::vec3(5.0f, 5.0f, 5.0f);
    
    std::cout << "Basketball object created at position: " << position.x << ", " 
              << position.y << ", " << position.z << std::endl;
}

Basketball::~Basketball() {
    delete model;
    glDeleteTextures(1, &textureID);
}

// Simple animation variables
static float animationTime = 0.0f;
static const float bouncePeriod = 2.0f; // Slower bouncing
static const float maxHeight = 2.0f;    // Reduce height range
static const float minHeight = -2.0f;   // Allow ball to go below "ground" for visibility

void Basketball::resetBall() {
    // Reset ball position and animation state
    position = glm::vec3(0.0f, 0.0f, 5.0f); // Very close to camera
    orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    animationTime = 0.0f;
    std::cout << "Basketball reset to position: " << position.x << ", " 
              << position.y << ", " << position.z << std::endl;
}

void Basketball::update(float deltaTime) {
    // Update animation time
    animationTime += deltaTime;
    
    // Very simple up/down movement - no damping for maximum visibility
    float bounce = sin(animationTime * 3.14159f / bouncePeriod); // -1 to 1 range
    float height = bounce * 2.0f; // Scale to make movement more obvious
    
    // Update position - keep z position fixed for visibility
    position.y = height;
    position.z = 5.0f;
    
    // Slowly rotate the ball for visual interest
    float rotationSpeed = 90.0f * deltaTime; // degrees per second
    glm::quat rotationDelta = glm::angleAxis(glm::radians(rotationSpeed), glm::vec3(0.0f, 1.0f, 0.0f));
    orientation = rotationDelta * orientation;
    orientation = glm::normalize(orientation);
    
    // Debug output
    static float debugTimer = 0.0f;
    debugTimer += deltaTime;
    if (debugTimer > 0.2f) { // More frequent output
        std::cout << "Ball position: (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
        debugTimer = 0.0f;
    }
}

void Basketball::draw(GLuint shaderID, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
    // Create model matrix components
    glm::mat4 transMatrix = MyMaths::createTranslationMatrix(position);
    glm::mat4 scaleMatrix = MyMaths::createScaleMatrix(scale);
    glm::mat4 rotMatrix = glm::mat4_cast(orientation); // Convert quaternion to rotation matrix

    // Order: Scale -> Rotate -> Translate
    glm::mat4 modelMatrix = transMatrix * rotMatrix * scaleMatrix;

    glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;

    glUniformMatrix4fv(glGetUniformLocation(shaderID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, &modelMatrix[0][0]);
    
    // Set texture to unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->textureID);
    glUniform1i(glGetUniformLocation(shaderID, "texture_diffuse"), 0);
    
    // Provide a dummy texture for the normal map to avoid errors
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, this->textureID); // Just reuse the diffuse texture
    glUniform1i(glGetUniformLocation(shaderID, "texture_normal"), 1);

    model->draw(shaderID);
} 