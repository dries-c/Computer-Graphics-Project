#ifndef OPENGL_PROJECT_SKYBOXTEXTURE_H
#define OPENGL_PROJECT_SKYBOXTEXTURE_H

#include <vector>
#include "../model/shader/Shader.h"

class SkyboxTexture {
public:
    explicit SkyboxTexture(std::vector<std::string> faces);
    void bind(Shader &shader);
};


#endif
