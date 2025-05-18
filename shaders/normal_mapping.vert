#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
// Tangent and bitangent not used since our OBJ doesn't include them

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Uniforms for lighting in world space
uniform vec3 pointLightPosWorld;
uniform vec3 viewPosWorld;
uniform vec3 dirLightDirectionWorld;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoord = aTexCoord;
    
    // Calculate normal in world space
    Normal = normalize(mat3(transpose(inverse(model))) * aNormal);
    
    // Basic lighting without normal mapping
    // We'll pass world space vectors directly
} 