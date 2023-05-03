#ifndef OPENGL_PROJECT_POINTLIGHT_H
#define OPENGL_PROJECT_POINTLIGHT_H


#include "LightSource.h"

class PointLight : public LightSource {
private:
    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;
public:
    PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic) : LightSource(ambient, diffuse, specular), position(position), constant(constant), linear(linear), quadratic(quadratic) {}
    PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic) : LightSource(ambient, diffuse, specular), constant(constant), linear(linear), quadratic(quadratic) {}
    void bind(Shader *shader, int index);
    void setPosition(glm::vec3 position);
};


#endif
