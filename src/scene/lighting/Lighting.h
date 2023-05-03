#ifndef OPENGL_PROJECT_LIGHTING_H
#define OPENGL_PROJECT_LIGHTING_H

#include "DirectionalLightSource.h"

class Lighting {
private:
    DirectionalLightSource *directionalLightSource;
    glm::vec3 cameraPosition = glm::vec3(0.0f);
public:
    Lighting();
    ~Lighting();

    void bind(Shader *shader) const;
    void updateCameraPosition(glm::vec3 cameraPosition);
};


#endif
