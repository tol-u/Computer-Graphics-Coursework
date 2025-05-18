#include "texture.hpp" // For the loadTexture declaration

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.hpp" // Path relative to common/ or adjust as needed if stb_image.h is elsewhere
                         // Assuming stb_image.hpp is in the common/ directory alongside texture.hpp/cpp

#include <stdio.h> // For printf
#include <GL/glew.h> // For OpenGL functions

unsigned int loadTexture(const char *path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    int width, height, nChannels;
    stbi_set_flip_vertically_on_load(true); // Good practice for OpenGL
    unsigned char *data = stbi_load(path, &width, &height, &nChannels, 0);
    
    if (data)
    {
        GLenum format;
        if (nChannels == 1)
            format = GL_RED;
        else if (nChannels == 3)
            format = GL_RGB;
        else if (nChannels == 4)
            format = GL_RGBA;
        else { // Should not happen with typical image formats
            printf("Texture %s has an unsupported number of channels: %d\n", path, nChannels);
            stbi_image_free(data);
            return 0; // Or some error indicator
        }
        
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        printf("Texture %s failed to load. Reason: %s\n", path, stbi_failure_reason());
    }

    stbi_image_free(data);
    
    return textureID;
} 