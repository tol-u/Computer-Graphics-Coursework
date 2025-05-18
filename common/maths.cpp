#include "maths.hpp"
#include <cmath> // For sin, cos

namespace MyMaths {

glm::mat4 createTranslationMatrix(const glm::vec3& translation) {
    glm::mat4 matrix(1.0f); // Identity matrix
    matrix[3][0] = translation.x;
    matrix[3][1] = translation.y;
    matrix[3][2] = translation.z;
    return matrix;
}

// Generic Axis-Angle Rotation
glm::mat4 createRotationMatrix(float angleDegrees, const glm::vec3& axis) {
    float angleRadians = glm::radians(angleDegrees);
    glm::vec3 normalizedAxis = glm::normalize(axis);
    float c = cos(angleRadians);
    float s = sin(angleRadians);
    float t = 1.0f - c;
    float x = normalizedAxis.x;
    float y = normalizedAxis.y;
    float z = normalizedAxis.z;

    glm::mat4 rotationMatrix(1.0f);
    rotationMatrix[0][0] = t * x * x + c;
    rotationMatrix[0][1] = t * x * y - s * z;
    rotationMatrix[0][2] = t * x * z + s * y;

    rotationMatrix[1][0] = t * x * y + s * z;
    rotationMatrix[1][1] = t * y * y + c;
    rotationMatrix[1][2] = t * y * z - s * x;

    rotationMatrix[2][0] = t * x * z - s * y;
    rotationMatrix[2][1] = t * y * z + s * x;
    rotationMatrix[2][2] = t * z * z + c;

    return rotationMatrix;
}

glm::mat4 createRotationMatrixX(float angleDegrees) {
    float angleRadians = glm::radians(angleDegrees);
    float c = cos(angleRadians);
    float s = sin(angleRadians);
    glm::mat4 matrix(1.0f);
    matrix[1][1] = c;
    matrix[1][2] = s; // Or -s depending on handedness and rotation direction convention
    matrix[2][1] = -s; // Or s
    matrix[2][2] = c;
    // GLM rotates counter-clockwise for positive angles looking down the axis.
    // Standard X rotation: 
    // 1  0   0   0
    // 0  cos -sin 0
    // 0  sin cos  0
    // 0  0   0   1
    // So, matrix[1][2] should be -s and matrix[2][1] should be s for standard CCW from +X
    matrix[1][2] = -s;
    matrix[2][1] = s;
    return matrix;
}

glm::mat4 createRotationMatrixY(float angleDegrees) {
    float angleRadians = glm::radians(angleDegrees);
    float c = cos(angleRadians);
    float s = sin(angleRadians);
    glm::mat4 matrix(1.0f);
    // Standard Y rotation: 
    // cos  0  sin  0
    // 0    1  0    0
    // -sin 0  cos  0
    // 0    0  0    1
    matrix[0][0] = c;
    matrix[0][2] = s;
    matrix[2][0] = -s;
    matrix[2][2] = c;
    return matrix;
}

glm::mat4 createRotationMatrixZ(float angleDegrees) {
    float angleRadians = glm::radians(angleDegrees);
    float c = cos(angleRadians);
    float s = sin(angleRadians);
    glm::mat4 matrix(1.0f);
    // Standard Z rotation: 
    // cos -sin 0  0
    // sin cos  0  0
    // 0   0    1  0
    // 0   0    0  1
    matrix[0][0] = c;
    matrix[0][1] = -s;
    matrix[1][0] = s;
    matrix[1][1] = c;
    return matrix;
}

glm::mat4 createScaleMatrix(const glm::vec3& scale) {
    glm::mat4 matrix(1.0f);
    matrix[0][0] = scale.x;
    matrix[1][1] = scale.y;
    matrix[2][2] = scale.z;
    return matrix;
}

} // namespace MyMaths
