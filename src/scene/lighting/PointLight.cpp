#include "PointLight.h"
#include <iostream>

void PointLight::bind(Shader *shader, int index) {
    std::string prefix = "pointLights[" + std::to_string(index) + "].";
    shader->setVec3(prefix + "position", position);
    shader->setFloat(prefix + "constant", constant);
    shader->setFloat(prefix + "linear", linear);
    shader->setFloat(prefix + "quadratic", quadratic);

    LightSource::bind(shader, prefix);
}

void PointLight::setPosition(glm::vec3 pos) {
    this->position = pos;
}
