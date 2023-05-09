#ifndef OPENGL_PROJECT_SPOTLIGHT_H
#define OPENGL_PROJECT_SPOTLIGHT_H

#include "PointLight.h"

class SpotLight : public LightSource {
private:
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 direction = glm::vec3(0.0f);
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;
public:
    SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 color, float cutOff, float outerCutOff, float constant, float linear, float quadratic);
    SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 color, float cutOff, float outerCutOff, float constant, float linear, float quadratic);

    void bind(Shader *shader, int index);
    void updatePosition(glm::vec3 position, glm::vec3 direction);
};


#endif
