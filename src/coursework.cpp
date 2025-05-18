#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <cmath>

// Camera class to replace GLM view matrix functions
class Camera {
public:
    // Camera attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    
    // Euler angles
    float Yaw;
    float Pitch;
    
    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    
    // Constructor
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
           float yaw = -90.0f, float pitch = 0.0f) : 
        Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(2.5f), MouseSensitivity(0.1f), Zoom(45.0f) {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    
    // Returns the view matrix calculated using Euler Angles
    glm::mat4 GetViewMatrix() {
        return lookAt(Position, Position + Front, Up);
    }
    
    // Custom lookAt function to replace glm::lookAt
    glm::mat4 lookAt(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp) {
        // Calculate the camera coordinate system
        glm::vec3 zaxis = glm::normalize(position - target);    // The "forward" vector
        glm::vec3 xaxis = glm::normalize(glm::cross(worldUp, zaxis)); // The "right" vector
        glm::vec3 yaxis = glm::cross(zaxis, xaxis);     // The "up" vector
        
        // Create a 4x4 orientation matrix from the right, up, and forward vectors
        glm::mat4 orientation = glm::mat4(1.0f);
        orientation[0][0] = xaxis.x;
        orientation[0][1] = yaxis.x;
        orientation[0][2] = zaxis.x;
        orientation[1][0] = xaxis.y;
        orientation[1][1] = yaxis.y;
        orientation[1][2] = zaxis.y;
        orientation[2][0] = xaxis.z;
        orientation[2][1] = yaxis.z;
        orientation[2][2] = zaxis.z;
        
        // Create a 4x4 translation matrix
        glm::mat4 translation = glm::mat4(1.0f);
        translation[3][0] = -position.x;
        translation[3][1] = -position.y;
        translation[3][2] = -position.z;
        
        // Return orientation * translation
        return orientation * translation;
    }
    
    // Processes input received from keyboard
    void ProcessKeyboard(int direction, float deltaTime) {
        float velocity = MovementSpeed * deltaTime;
        if (direction == 0) // FORWARD
            Position += Front * velocity;
        if (direction == 1) // BACKWARD
            Position -= Front * velocity;
        if (direction == 2) // LEFT
            Position -= Right * velocity;
        if (direction == 3) // RIGHT
            Position += Right * velocity;
    }
    
    // Processes input received from a mouse input system
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;
        
        Yaw   += xoffset;
        Pitch += yoffset;
        
        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch) {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }
        
        // Update Front, Right and Up vectors using the updated Euler angles
        updateCameraVectors();
    }
    
private:
    // Calculates the front vector from the Camera's Euler angles
    void updateCameraVectors() {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        
        // Also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};

// Custom projection matrix function to replace glm::perspective
glm::mat4 perspective(float fov, float aspectRatio, float near, float far) {
    glm::mat4 projection = glm::mat4(0.0f);
    
    float tanHalfFovy = tan(fov / 2.0f);
    
    projection[0][0] = 1.0f / (aspectRatio * tanHalfFovy);
    projection[1][1] = 1.0f / tanHalfFovy;
    projection[2][2] = -(far + near) / (far - near);
    projection[2][3] = -1.0f;
    projection[3][2] = -(2.0f * far * near) / (far - near);
    
    return projection;
}

// Camera
Camera camera(glm::vec3(0.0f, 1.0f, 5.0f));
bool firstMouse = true;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;

// Function declarations
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window, float deltaTime);

// Directional enum for camera movement
enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Main function
int main(void)
{
    // Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }
    
    // Set up window
    glfwWindowHint(GLFW_SAMPLES, 4); // Anti-aliasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Simple Bouncing Basketball", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // Set up mouse capture
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    
    // Initialize GLEW
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
    
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    
    // Set up vertex data for a simple sphere (basketball)
    const int segments = 16;
    const int rings = 16;
    const float radius = 0.5f;
    
    // Calculate vertices count
    int vertexCount = (rings + 1) * (segments + 1);
    int indexCount = rings * segments * 6;
    
    std::vector<float> vertices(vertexCount * 8); // position(3) + normal(3) + texcoord(2)
    std::vector<unsigned int> indices(indexCount);
    
    // Generate sphere vertices
    int vertIndex = 0;
    for (int ring = 0; ring <= rings; ring++) {
        float theta = ring * 3.14159f / rings;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);
        
        for (int segment = 0; segment <= segments; segment++) {
            float phi = segment * 2.0f * 3.14159f / segments;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);
            
            // Position
            float x = cosPhi * sinTheta;
            float y = cosTheta;
            float z = sinPhi * sinTheta;
            
            vertices[vertIndex++] = radius * x;
            vertices[vertIndex++] = radius * y;
            vertices[vertIndex++] = radius * z;
            
            // Normal
            vertices[vertIndex++] = x;
            vertices[vertIndex++] = y;
            vertices[vertIndex++] = z;
            
            // Texcoord
            vertices[vertIndex++] = (float)segment / segments;
            vertices[vertIndex++] = (float)ring / rings;
        }
    }
    
    // Generate indices
    int indexIndex = 0;
    for (int ring = 0; ring < rings; ring++) {
        int ringVerts = segments + 1;
        for (int segment = 0; segment < segments; segment++) {
            indices[indexIndex++] = ring * ringVerts + segment;
            indices[indexIndex++] = (ring + 1) * ringVerts + segment;
            indices[indexIndex++] = ring * ringVerts + segment + 1;
            
            indices[indexIndex++] = ring * ringVerts + segment + 1;
            indices[indexIndex++] = (ring + 1) * ringVerts + segment;
            indices[indexIndex++] = (ring + 1) * ringVerts + segment + 1;
        }
    }
    
    // Create floor vertices
    float floorSize = 10.0f;
    std::vector<float> floorVertices;
    floorVertices.push_back(-floorSize); floorVertices.push_back(0.0f); floorVertices.push_back(-floorSize);
    floorVertices.push_back(0.0f); floorVertices.push_back(1.0f); floorVertices.push_back(0.0f);
    floorVertices.push_back(0.0f); floorVertices.push_back(0.0f);
    
    floorVertices.push_back(floorSize); floorVertices.push_back(0.0f); floorVertices.push_back(-floorSize);
    floorVertices.push_back(0.0f); floorVertices.push_back(1.0f); floorVertices.push_back(0.0f);
    floorVertices.push_back(floorSize); floorVertices.push_back(0.0f);
    
    floorVertices.push_back(floorSize); floorVertices.push_back(0.0f); floorVertices.push_back(floorSize);
    floorVertices.push_back(0.0f); floorVertices.push_back(1.0f); floorVertices.push_back(0.0f);
    floorVertices.push_back(floorSize); floorVertices.push_back(floorSize);
    
    floorVertices.push_back(-floorSize); floorVertices.push_back(0.0f); floorVertices.push_back(floorSize);
    floorVertices.push_back(0.0f); floorVertices.push_back(1.0f); floorVertices.push_back(0.0f);
    floorVertices.push_back(0.0f); floorVertices.push_back(floorSize);
    
    std::vector<unsigned int> floorIndices;
    floorIndices.push_back(0); floorIndices.push_back(1); floorIndices.push_back(2);
    floorIndices.push_back(2); floorIndices.push_back(3); floorIndices.push_back(0);
    
    // Create basketball hoop vertices
    std::vector<float> hoopVertices;
    std::vector<unsigned int> hoopIndices;
    
    // Backboard
    float boardWidth = 1.8f;
    float boardHeight = 1.2f;
    float boardThickness = 0.05f;
    float boardY = 3.0f;
    float boardZ = -5.0f;
    
    // Front face (facing player)
    // bottom-left
    hoopVertices.push_back(-boardWidth/2); hoopVertices.push_back(boardY-boardHeight/2); hoopVertices.push_back(boardZ);
    hoopVertices.push_back(0.0f); hoopVertices.push_back(0.0f); hoopVertices.push_back(1.0f);
    hoopVertices.push_back(0.0f); hoopVertices.push_back(0.0f);
    
    // bottom-right
    hoopVertices.push_back(boardWidth/2); hoopVertices.push_back(boardY-boardHeight/2); hoopVertices.push_back(boardZ);
    hoopVertices.push_back(0.0f); hoopVertices.push_back(0.0f); hoopVertices.push_back(1.0f);
    hoopVertices.push_back(1.0f); hoopVertices.push_back(0.0f);
    
    // top-right
    hoopVertices.push_back(boardWidth/2); hoopVertices.push_back(boardY+boardHeight/2); hoopVertices.push_back(boardZ);
    hoopVertices.push_back(0.0f); hoopVertices.push_back(0.0f); hoopVertices.push_back(1.0f);
    hoopVertices.push_back(1.0f); hoopVertices.push_back(1.0f);
    
    // top-left
    hoopVertices.push_back(-boardWidth/2); hoopVertices.push_back(boardY+boardHeight/2); hoopVertices.push_back(boardZ);
    hoopVertices.push_back(0.0f); hoopVertices.push_back(0.0f); hoopVertices.push_back(1.0f);
    hoopVertices.push_back(0.0f); hoopVertices.push_back(1.0f);
    
    // Back face
    // bottom-left
    hoopVertices.push_back(-boardWidth/2); hoopVertices.push_back(boardY-boardHeight/2); hoopVertices.push_back(boardZ-boardThickness);
    hoopVertices.push_back(0.0f); hoopVertices.push_back(0.0f); hoopVertices.push_back(-1.0f);
    hoopVertices.push_back(0.0f); hoopVertices.push_back(0.0f);
    
    // bottom-right
    hoopVertices.push_back(boardWidth/2); hoopVertices.push_back(boardY-boardHeight/2); hoopVertices.push_back(boardZ-boardThickness);
    hoopVertices.push_back(0.0f); hoopVertices.push_back(0.0f); hoopVertices.push_back(-1.0f);
    hoopVertices.push_back(1.0f); hoopVertices.push_back(0.0f);
    
    // top-right
    hoopVertices.push_back(boardWidth/2); hoopVertices.push_back(boardY+boardHeight/2); hoopVertices.push_back(boardZ-boardThickness);
    hoopVertices.push_back(0.0f); hoopVertices.push_back(0.0f); hoopVertices.push_back(-1.0f);
    hoopVertices.push_back(1.0f); hoopVertices.push_back(1.0f);
    
    // top-left
    hoopVertices.push_back(-boardWidth/2); hoopVertices.push_back(boardY+boardHeight/2); hoopVertices.push_back(boardZ-boardThickness);
    hoopVertices.push_back(0.0f); hoopVertices.push_back(0.0f); hoopVertices.push_back(-1.0f);
    hoopVertices.push_back(0.0f); hoopVertices.push_back(1.0f);
    
    // Add indices for backboard (6 vertices for front, 6 for back)
    // front face
    hoopIndices.push_back(0); hoopIndices.push_back(1); hoopIndices.push_back(2);
    hoopIndices.push_back(2); hoopIndices.push_back(3); hoopIndices.push_back(0);
    
    // back face
    hoopIndices.push_back(4); hoopIndices.push_back(5); hoopIndices.push_back(6);
    hoopIndices.push_back(6); hoopIndices.push_back(7); hoopIndices.push_back(4);
    
    // Create rim
    float rimRadius = 0.45f;
    float rimThickness = 0.05f;
    int rimSegments = 32;
    float rimY = boardY - 0.15f;
    float rimZ = boardZ + 0.6f;
    
    // Base index for rim vertices
    unsigned int rimBaseIndex = hoopVertices.size() / 8;
    
    // Create rim vertices
    for (int i = 0; i < rimSegments; i++) {
        float angle = 2.0f * 3.14159f * i / rimSegments;
        float nextAngle = 2.0f * 3.14159f * (i + 1) / rimSegments;
        
        float x = rimRadius * cos(angle);
        float z = rimRadius * sin(angle);
        float nextX = rimRadius * cos(nextAngle);
        float nextZ = rimRadius * sin(nextAngle);
        
        // Top of rim - first vertex
        hoopVertices.push_back(x); hoopVertices.push_back(rimY); hoopVertices.push_back(rimZ + z);
        hoopVertices.push_back(0.0f); hoopVertices.push_back(1.0f); hoopVertices.push_back(0.0f);
        hoopVertices.push_back(0.0f); hoopVertices.push_back(0.0f);
        
        // Top of rim - second vertex
        hoopVertices.push_back(nextX); hoopVertices.push_back(rimY); hoopVertices.push_back(rimZ + nextZ);
        hoopVertices.push_back(0.0f); hoopVertices.push_back(1.0f); hoopVertices.push_back(0.0f);
        hoopVertices.push_back(0.0f); hoopVertices.push_back(0.0f);
        
        // Top of rim - center vertex
        hoopVertices.push_back(0.0f); hoopVertices.push_back(rimY); hoopVertices.push_back(rimZ);
        hoopVertices.push_back(0.0f); hoopVertices.push_back(1.0f); hoopVertices.push_back(0.0f);
        hoopVertices.push_back(0.0f); hoopVertices.push_back(0.0f);
        
        // Add indices for this triangle
        hoopIndices.push_back(rimBaseIndex + i * 3);
        hoopIndices.push_back(rimBaseIndex + i * 3 + 1);
        hoopIndices.push_back(rimBaseIndex + i * 3 + 2);
    }
    
    // Create basketball VAO, VBO and EBO
    GLuint basketballVAO, basketballVBO, basketballEBO;
    glGenVertexArrays(1, &basketballVAO);
    glGenBuffers(1, &basketballVBO);
    glGenBuffers(1, &basketballEBO);
    
    glBindVertexArray(basketballVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, basketballVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, basketballEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Texcoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    // Create floor VAO, VBO and EBO
    GLuint floorVAO, floorVBO, floorEBO;
    glGenVertexArrays(1, &floorVAO);
    glGenBuffers(1, &floorVBO);
    glGenBuffers(1, &floorEBO);
    
    glBindVertexArray(floorVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, floorVertices.size() * sizeof(float), floorVertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, floorIndices.size() * sizeof(unsigned int), floorIndices.data(), GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Texcoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // Create hoop VAO, VBO and EBO
    GLuint hoopVAO, hoopVBO, hoopEBO;
    glGenVertexArrays(1, &hoopVAO);
    glGenBuffers(1, &hoopVBO);
    glGenBuffers(1, &hoopEBO);
    
    glBindVertexArray(hoopVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, hoopVBO);
    glBufferData(GL_ARRAY_BUFFER, hoopVertices.size() * sizeof(float), hoopVertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, hoopEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, hoopIndices.size() * sizeof(unsigned int), hoopIndices.data(), GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Texcoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // Simple vertex shader with normal mapping
    const char* vertexShaderSource = 
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aNormal;\n"
        "layout (location = 2) in vec2 aTexCoord;\n"
        "\n"
        "out vec3 FragPos;\n"
        "out vec3 Normal;\n"
        "out vec2 TexCoord;\n"
        "out mat3 TBN;\n"  // Added for normal mapping
        "\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    FragPos = vec3(model * vec4(aPos, 1.0));\n"
        "    Normal = mat3(transpose(inverse(model))) * aNormal;\n"
        "    TexCoord = aTexCoord;\n"
        "    \n"
        "    // Calculate tangent vectors for normal mapping\n"
        "    vec3 T = normalize(vec3(model * vec4(1.0, 0.0, 0.0, 0.0)));\n"
        "    vec3 B = normalize(vec3(model * vec4(0.0, 0.0, 1.0, 0.0)));\n"
        "    vec3 N = normalize(Normal);\n"
        "    TBN = mat3(T, B, N);\n"
        "    \n"
        "    gl_Position = projection * view * vec4(FragPos, 1.0);\n"
        "}\n";
    
    // Simple fragment shader with normal mapping
    const char* fragmentShaderSource = 
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "\n"
        "in vec3 FragPos;\n"
        "in vec3 Normal;\n"
        "in vec2 TexCoord;\n"
        "in mat3 TBN;\n"  // Added for normal mapping
        "\n"
        "uniform vec3 lightPos;\n"
        "uniform vec3 viewPos;\n"
        "uniform vec3 objectColor;\n"
        "uniform bool useNormalMap;\n"  // Flag to toggle normal mapping
        "\n"
        "// Procedural normal map for basketball\n"
        "vec3 calculateNormalFromTexture(vec2 texCoord) {\n"
        "    // Basic normal that points outward\n"
        "    vec3 normal = vec3(0.0, 0.0, 1.0);\n"
        "    \n"
        "    // Add bumps for basketball texture\n"
        "    float bumpIntensity = 0.3;\n"
        "    \n"
        "    // Create a bumpy pattern based on a grid\n"
        "    float gridSize = 16.0;\n"
        "    vec2 cell = fract(texCoord * gridSize);\n"
        "    vec2 cellCenter = abs(cell - 0.5);\n"
        "    float distanceFromCenter = length(cellCenter);\n"
        "    \n"
        "    // Create bump effect\n"
        "    float bumpHeight = smoothstep(0.4, 0.0, distanceFromCenter) * bumpIntensity;\n"
        "    \n"
        "    // Create normal vector from bump\n"
        "    vec3 perturbedNormal = normal;\n"
        "    perturbedNormal.x = (cell.x - 0.5) * bumpHeight;\n"
        "    perturbedNormal.y = (cell.y - 0.5) * bumpHeight;\n"
        "    perturbedNormal = normalize(perturbedNormal);\n"
        "    \n"
        "    return perturbedNormal;\n"
        "}\n"
        "\n"
        "void main()\n"
        "{\n"
        "    // Get the normal from normal mapping or use the interpolated normal\n"
        "    vec3 norm;\n"
        "    if (useNormalMap && objectColor.r > 0.9 && objectColor.g > 0.4) { // Only apply to orange basketball\n"
        "        vec3 normalFromTexture = calculateNormalFromTexture(TexCoord);\n"
        "        norm = normalize(TBN * normalFromTexture);\n"
        "    } else {\n"
        "        norm = normalize(Normal);\n"
        "    }\n"
        "    \n"
        "    // Ambient\n"
        "    float ambientStrength = 0.3;\n"
        "    vec3 ambient = ambientStrength * vec3(1.0);\n"
        "    \n"
        "    // Diffuse\n"
        "    vec3 lightDir = normalize(lightPos - FragPos);\n"
        "    float diff = max(dot(norm, lightDir), 0.0);\n"
        "    vec3 diffuse = diff * vec3(1.0);\n"
        "    \n"
        "    // Specular\n"
        "    float specularStrength = 0.5;\n"
        "    vec3 viewDir = normalize(viewPos - FragPos);\n"
        "    vec3 reflectDir = reflect(-lightDir, norm);\n"
        "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
        "    vec3 specular = specularStrength * spec * vec3(1.0);\n"
        "    \n"
        "    // Special handling for basketball stripes\n"
        "    float stripeFactor = 0.0;\n"
        "    if (objectColor.r > 0.9 && objectColor.g > 0.4) { // Only apply to orange basketball\n"
        "        // Horizontal stripes\n"
        "        if (mod(TexCoord.y * 8.0, 1.0) > 0.8) {\n"
        "            stripeFactor = 0.5;\n"
        "        }\n"
        "        \n"
        "        // Vertical stripes\n"
        "        if (mod(TexCoord.x * 8.0, 1.0) > 0.8) {\n"
        "            stripeFactor = 0.5;\n"
        "        }\n"
        "    }\n"
        "    \n"
        "    // Combine results\n"
        "    vec3 result = (ambient + diffuse + specular) * objectColor * (1.0 - stripeFactor);\n"
        "    FragColor = vec4(result, 1.0);\n"
        "}\n";
    
    // Compile shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    // Check for errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    // Set background color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    
    // Get uniform locations
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    GLint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
    GLint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
    GLint objectColorLoc = glGetUniformLocation(shaderProgram, "objectColor");
    GLint useNormalMapLoc = glGetUniformLocation(shaderProgram, "useNormalMap");
    
    // Bouncing parameters
    float g = 9.8f; // Gravity
    float floor_y = 0.0f; // Floor position
    float height = 2.0f; // Initial height
    float restitution = 0.8f; // Bounciness factor
    float velocity = 0.0f; // Initial velocity
    
    // Timing
    float lastTime = glfwGetTime();
    float deltaTime = 0.0f;
    
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Calculate delta time
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        
        // Process input
        processInput(window, deltaTime);
        
        // Update basketball position with physics
        velocity -= g * deltaTime; // Apply gravity
        height += velocity * deltaTime; // Apply velocity
        
        // Bounce when hitting the floor
        if (height <= floor_y + radius) {
            height = floor_y + radius; // Correct position
            velocity = -velocity * restitution; // Bounce with energy loss
            
            // Stop bouncing if velocity is too low
            if (fabs(velocity) < 0.2f) {
                velocity = 0.0f;
                height = floor_y + radius;
            }
        }
        
        // Reset if ball stops and space is pressed
        if (velocity == 0.0f && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            height = 2.0f;
            velocity = 0.0f;
        }
        
        // Clear buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Use shader
        glUseProgram(shaderProgram);
        
        // Custom perspective and view matrices
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        
        // Set uniforms
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
        glUniform3f(lightPosLoc, 2.0f, 5.0f, 5.0f);
        glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);
        
        // Draw floor
        glBindVertexArray(floorVAO);
        glm::mat4 model = glm::mat4(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
        glUniform3f(objectColorLoc, 0.5f, 0.5f, 0.5f); // Gray floor
        glUniform1i(useNormalMapLoc, 0); // No normal mapping for floor
        glDrawElements(GL_TRIANGLES, floorIndices.size(), GL_UNSIGNED_INT, 0);
        
        // Draw basketball hoop
        glBindVertexArray(hoopVAO);
        model = glm::mat4(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
        glUniform3f(objectColorLoc, 0.2f, 0.2f, 0.2f); // Dark gray hoop
        glUniform1i(useNormalMapLoc, 0); // No normal mapping for hoop
        glDrawElements(GL_TRIANGLES, hoopIndices.size(), GL_UNSIGNED_INT, 0);
        
        // Draw basketball
        glBindVertexArray(basketballVAO);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, height, 0.0f));
        
        // Add slight rotation for realism
        model = glm::rotate(model, currentTime * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
        glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.0f); // Orange basketball
        glUniform1i(useNormalMapLoc, 1); // Enable normal mapping for basketball
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        
        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        // Simple debug output
        if (int(currentTime) % 1 == 0 && int(currentTime) != int(lastTime)) {
            std::cout << "Ball height: " << height << ", velocity: " << velocity << std::endl;
            std::cout << "Camera position: (" << camera.Position.x << ", " 
                      << camera.Position.y << ", " << camera.Position.z << ")" << std::endl;
        }
    }
    
    // Clean up
    glDeleteVertexArrays(1, &basketballVAO);
    glDeleteBuffers(1, &basketballVBO);
    glDeleteBuffers(1, &basketballEBO);
    glDeleteVertexArrays(1, &floorVAO);
    glDeleteBuffers(1, &floorVBO);
    glDeleteBuffers(1, &floorEBO);
    glDeleteVertexArrays(1, &hoopVAO);
    glDeleteBuffers(1, &hoopVBO);
    glDeleteBuffers(1, &hoopEBO);
    glDeleteProgram(shaderProgram);
    
    glfwTerminate();
    return 0;
}

// Mouse callback
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to top
    
    lastX = xpos;
    lastY = ypos;
    
    camera.ProcessMouseMovement(xoffset, yoffset);
}

// Process keyboard input
void processInput(GLFWwindow* window, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
} 