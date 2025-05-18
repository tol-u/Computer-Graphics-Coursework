#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse;
uniform vec3 objectColor;

// Light properties
uniform vec3 LightPosition_worldspace;
uniform vec3 LightColor;
uniform float LightPower;
uniform vec3 AmbientLightColor;
uniform vec3 EyePosition_worldspace;

void main()
{
    // Material properties
    vec3 diffuseColor = texture(texture_diffuse, TexCoord).rgb * objectColor;
    vec3 ambientColor = diffuseColor * AmbientLightColor;
    vec3 specularColor = vec3(0.5, 0.5, 0.5); // Medium gray specular
    float shininess = 32.0;

    // Direction vectors
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(LightPosition_worldspace - FragPos);
    vec3 viewDir = normalize(EyePosition_worldspace - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    // Diffuse lighting
    float diff = max(dot(normal, lightDir), 0.0);
    
    // Specular lighting using Blinn-Phong
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    
    // Attenuation - make this very weak to ensure lighting remains strong
    float distance = length(LightPosition_worldspace - FragPos);
    float attenuation = 1.0 / (1.0 + 0.001 * distance + 0.00001 * distance * distance);
    
    // Calculate lighting components with very little attenuation
    vec3 ambient = ambientColor;
    vec3 diffuse = LightColor * diff * diffuseColor * LightPower;
    vec3 specular = LightColor * spec * specularColor * LightPower;
    
    // Apply very weak attenuation to ensure visibility
    diffuse *= attenuation;
    specular *= attenuation;
    
    // Combine lighting components
    vec3 result = ambient + diffuse + specular;
    
    // Ensure result doesn't exceed 1.0 (optional clamping)
    result = min(result, vec3(1.0, 1.0, 1.0));
    
    FragColor = vec4(result, 1.0);
}
