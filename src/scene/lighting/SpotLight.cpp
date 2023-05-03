#include "SpotLight.h"

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                     float cutOff, float outerCutOff, float constant, float linear, float quadratic) : LightSource(ambient, diffuse, specular),
                                                                                                      position(position),
                                                                                                      direction(direction),
                                                                                                      cutOff(cutOff),
                                                                                                      outerCutOff(outerCutOff),
                                                                                                      constant(constant),
                                                                                                      linear(linear),
                                                                                                      quadratic(quadratic) {}

SpotLight::SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float cutOff, float outerCutOff,
                     float constant, float linear, float quadratic) : LightSource(ambient, diffuse, specular),
                                                                       cutOff(cutOff),
                                                                       outerCutOff(outerCutOff),
                                                                       constant(constant),
                                                                       linear(linear),
                                                                       quadratic(quadratic) {}

void SpotLight::bind(Shader *shader, int index) {
    if(index > 0) {
        throw std::runtime_error("SpotLight::bind() only supports one light source");
    }

    std::string prefix = "spotLight.";
    shader->setVec3(prefix + "position", position);
    shader->setVec3(prefix + "direction", direction);
    shader->setFloat(prefix + "cutOff", cutOff);
    shader->setFloat(prefix + "outerCutOff", outerCutOff);
    shader->setFloat(prefix + "constant", constant);
    shader->setFloat(prefix + "linear", linear);
    shader->setFloat(prefix + "quadratic", quadratic);

    LightSource::bind(shader, prefix);
}

void SpotLight::updatePosition(glm::vec3 position, glm::vec3 direction) {
    this->position = position;
    this->direction = direction;
}
