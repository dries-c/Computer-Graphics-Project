#ifndef OPENGL_PROJECT_DIRECTIONALLIGHT_H
#define OPENGL_PROJECT_DIRECTIONALLIGHT_H


#include "LightSource.h"

class DirectionalLight : public LightSource {
private:
    glm::vec3 direction;

public:
    DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

    void bind(Shader *shader, int index);
};


#endif
