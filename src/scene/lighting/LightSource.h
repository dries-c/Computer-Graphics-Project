#ifndef OPENGL_PROJECT_LIGHTSOURCE_H
#define OPENGL_PROJECT_LIGHTSOURCE_H


#include "glm/vec3.hpp"
#include "../model/shader/Shader.h"

class LightSource {
public:
    LightSource(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 color) : color(color), ambient(ambient), diffuse(diffuse), specular(specular) {}
    virtual void bind(Shader *shader, std::string prefix);

protected:
    glm::vec3 color;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

#endif
