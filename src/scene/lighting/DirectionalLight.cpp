#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse,
                                   glm::vec3 specular) : LightSource(ambient, diffuse, specular),
                                                                     direction(direction) {}

void DirectionalLight::bind(Shader *shader, int index) {
    if(index > 0) {
        throw std::runtime_error("DirectionalLight::bind() only supports one light source");
    }

    std::string prefix = "directionalLight.";
    shader->setVec3(prefix + "direction", direction);
    LightSource::bind(shader, prefix);
}