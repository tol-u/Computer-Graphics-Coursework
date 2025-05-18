#include "rim.hpp"
#include "../common/texture.hpp"
#include "../common/maths.hpp"

Rim::Rim(const std::string& modelPath, const std::string& texturePath) {
    model = new Model(modelPath.c_str());
    this->textureID = loadTexture(texturePath.c_str());

    // Initial position for the rim
    position = glm::vec3(0.0f, 3.0f, -5.0f); 
    rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

Rim::~Rim() {
    delete model;
    glDeleteTextures(1, &textureID);
}

void Rim::update(float deltaTime) {
    // Example: Rotate around Z axis slowly
    rotation.z += 5.0f * deltaTime;
    if (rotation.z > 360.0f) rotation.z -= 360.0f;
}

void Rim::draw(GLuint shaderID, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::mat4 scaleMatrix = MyMaths::createScaleMatrix(scale);
    glm::mat4 rotXMatrix = MyMaths::createRotationMatrixX(rotation.x);
    glm::mat4 rotYMatrix = MyMaths::createRotationMatrixY(rotation.y);
    glm::mat4 rotZMatrix = MyMaths::createRotationMatrixZ(rotation.z);
    glm::mat4 transMatrix = MyMaths::createTranslationMatrix(position);

    modelMatrix = transMatrix * rotZMatrix * rotYMatrix * rotXMatrix * scaleMatrix;

    glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;

    glUniformMatrix4fv(glGetUniformLocation(shaderID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, &modelMatrix[0][0]);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->textureID);

    model->draw(shaderID);
} 