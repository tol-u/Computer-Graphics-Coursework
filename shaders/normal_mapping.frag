#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse; // Diffuse map
uniform sampler2D texture_normal;  // Not used but kept for API compatibility

uniform vec3 objectColor; // For tinting or if no diffuse texture

// Light properties (world space)
uniform vec3 pointLightPosWorld; 
uniform vec3 pointLightColor;
uniform vec3 dirLightDirectionWorld; // Direction FROM light source
uniform vec3 dirLightColor;
uniform vec3 viewPosWorld; // Camera position in world space

// Shared lighting parameters
const float ambientStrength = 0.2;
const float specularStrength = 0.5;
const int shininess = 32;

void main()
{
    // Use normal directly from vertex shader (normalized in world space)
    vec3 norm = normalize(Normal);
    
    // Calculate view direction in world space
    vec3 viewDir = normalize(viewPosWorld - FragPos);
    
    // === Point Light Calculation in World Space ===
    vec3 lightDir = normalize(pointLightPosWorld - FragPos);
    
    // Ambient
    vec3 ambient = ambientStrength * pointLightColor;
    
    // Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * pointLightColor;
    
    // Specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * pointLightColor;
    
    // === Directional Light Calculation in World Space ===
    vec3 lightDirDirectional = normalize(-dirLightDirectionWorld);
    
    // Diffuse (Directional)
    float diffDirectional = max(dot(norm, lightDirDirectional), 0.0);
    vec3 diffuseDirectional = diffDirectional * dirLightColor;
    
    // Specular (Directional)
    vec3 reflectDirDirectional = reflect(-lightDirDirectional, norm);
    float specDirectional = pow(max(dot(viewDir, reflectDirDirectional), 0.0), shininess);
    vec3 specularDirectional = specularStrength * specDirectional * dirLightColor;
    
    // Combine all lighting
    vec3 result = (ambient + diffuse + specular + diffuseDirectional + specularDirectional) * 
                  texture(texture_diffuse, TexCoord).rgb * objectColor;
    
    FragColor = vec4(result, 1.0);
} 