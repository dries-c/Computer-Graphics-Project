#define STB_IMAGE_IMPLEMENTATION

#include "Texture.h"

#include "stb_image.h"
#include "../../../utils/FileUtils.h"
#include <iostream>

Texture::Texture(const char *path) {
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    // load image, create texture
    int width, height, nrChannels;

    std::string fullPath = RESOURCES_PATH + path;
    unsigned char *data = stbi_load(fullPath.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Texture " << path << " failed to load" << std::endl;
    }

    stbi_image_free(data);
}

void Texture::bind(const Shader &shader, const std::string &name, int textureUnit) const {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    shader.setInt(name, textureUnit);
    glBindTexture(GL_TEXTURE_2D, ID);
}
