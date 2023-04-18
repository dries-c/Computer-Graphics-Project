#ifndef OPENGL_PROJECT_TEXTURE_H
#define OPENGL_PROJECT_TEXTURE_H

#include "glad/glad.h"
#include "../../model/shader/Shader.h"

class Texture {
public:
    unsigned int ID;

    explicit Texture(const char* path, int sWrap = GL_REPEAT, int tWrap = GL_REPEAT, int minFilter = GL_LINEAR, int magFilter = GL_LINEAR);
    void bind(Shader shader, int textureUnit = 0) const;
};


#endif
