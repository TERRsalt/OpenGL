#pragma once

#include <print>

#include <GL/glew.h>

#include <stb_image.h>

class Texture {
public:
    unsigned int textureId = 0;

    Texture(const std::string &textureFilePath, int filterMode) {
        int width, height, numberOfChannels;
        unsigned char *data;

        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);

        data = stbi_load(textureFilePath.c_str(), &width, &height, &numberOfChannels, 0);
        if (data) {
            int format = (numberOfChannels == 4)? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else std::println("Failed to load the texture");

        stbi_image_free(data);
    }

    void remove() const {glDeleteTextures(1, &textureId);}

    void bind(unsigned int textureUnit) const {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, textureId);
    }
};