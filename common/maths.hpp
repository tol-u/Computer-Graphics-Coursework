#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp> // For glm::pi

// Custom Math Functions
namespace MyMaths {

    glm::mat4 createTranslationMatrix(const glm::vec3& translation);
    glm::mat4 createRotationMatrix(float angleDegrees, const glm::vec3& axis);
    // Overload for Euler angles (common order: ZYX for yaw, pitch, roll if needed, or individual axis)
    glm::mat4 createRotationMatrixX(float angleDegrees);
    glm::mat4 createRotationMatrixY(float angleDegrees);
    glm::mat4 createRotationMatrixZ(float angleDegrees);
    glm::mat4 createScaleMatrix(const glm::vec3& scale);

    // Later, we can add:
    // glm::mat4 createLookAtMatrix(const glm::vec3& position, const glm::vec3& target, const glm::vec3& worldUp);
    // glm::mat4 createPerspectiveMatrix(float fovDegrees, float aspectRatio, float nearPlane, float farPlane);

} // namespace MyMaths
