#include "SkyboxTexture.h"
#include "glad/glad.h"
#include "../../utils/FileUtils.h"
#include "../mesh/texture/stb_image.h"
#include <iostream>

SkyboxTexture::SkyboxTexture(std::vector<std::string> faces) {
    unsigned int ID;

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++){
        std::string fullPath = RESOURCES_PATH + "textures/skybox/" + faces[i];

        unsigned char *data = stbi_load(fullPath.c_str(), &width, &height, &nrChannels, 0);
        if(data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        } else {
            std::cout << "Skybox texture " << faces[i] << " failed to load" << std::endl;
        }
        stbi_image_free(data);
    }
}

void SkyboxTexture::bind(Shader &shader) {
    shader.setInt("skybox", 0);
}
