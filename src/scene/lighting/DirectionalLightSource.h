#ifndef OPENGL_PROJECT_DIRECTIONALLIGHTSOURCE_H
#define OPENGL_PROJECT_DIRECTIONALLIGHTSOURCE_H


#include "LightSource.h"

class DirectionalLightSource : public LightSource {
private:
    glm::vec3 direction;

public:
    DirectionalLightSource(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

    void bind(Shader *shader, int index) override;
};


#endif
