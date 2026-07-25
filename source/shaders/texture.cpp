#include <print>

#include <GL/glew.h>
#include "stb_image.h"

unsigned int texture(const char *textureFilePath, int nearestOrLinear) {
    unsigned int currentTexture;

    int width, height, numberOfChannels;
    unsigned char *data;

    glGenTextures(1, &currentTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, currentTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, nearestOrLinear);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, nearestOrLinear);

    data = stbi_load(textureFilePath, &width, &height, &numberOfChannels, 0);
    if (data) {
        int format = (numberOfChannels == 4)? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else std::println("Failed to load the texture");

    stbi_image_free(data);

    return currentTexture;
}