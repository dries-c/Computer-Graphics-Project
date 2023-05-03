#include "DirectionalLightSource.h"

DirectionalLightSource::DirectionalLightSource(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse,
                                               glm::vec3 specular) : LightSource(ambient, diffuse, specular),
                                                                     direction(direction) {}

void DirectionalLightSource::bind(Shader *shader, int index) {
    if(index > 0) {
        throw std::runtime_error("DirectionalLightSource::bind() only supports one light source");
    }

    shader->setVec3("directionalLight.direction", direction);
    shader->setVec3("directionalLight.ambient", ambient);
    shader->setVec3("directionalLight.diffuse", diffuse);
    shader->setVec3("directionalLight.specular", specular);
}