#ifndef OPENGL_PROJECT_TEXTURE_H
#define OPENGL_PROJECT_TEXTURE_H

#include "glad/glad.h"
#include "../../model/shader/Shader.h"

class Texture {
public:
    unsigned int ID;

    explicit Texture(const char* path);
    void bind(const Shader &shader, const std::string &name, int textureUnit) const;
};


#endif
