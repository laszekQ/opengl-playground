#include "../../include/other/stb_image.h"
#include "../../include/glad/glad.h"
#include <iostream>
#include "../../include/other/classes/Texture.h"

Texture2D::Texture2D(const char* path, int desired_channels)
{
    glGenTextures(1, &ID);

    glBindTexture(GL_TEXTURE_2D, ID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, desired_channels);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cout << "Failed to load texture " << path << std::endl;


    stbi_image_free(data);
}

unsigned int Texture2D::getId()
{
    return ID;
}

void Texture2D::setParameteri(unsigned int option, unsigned int mode)
{
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexParameteri(GL_TEXTURE_2D, option, mode);
}
