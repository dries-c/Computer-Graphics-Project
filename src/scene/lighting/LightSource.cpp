#include "LightSource.h"

void LightSource::bind(Shader *shader, std::string prefix) {
    shader->setVec3(prefix + "base.ambient", ambient);
    shader->setVec3(prefix + "base.diffuse", diffuse);
    shader->setVec3(prefix + "base.specular", specular);
}
