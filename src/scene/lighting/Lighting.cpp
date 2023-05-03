#include "Lighting.h"

void Lighting::updateCameraPosition(glm::vec3 pos) {
    this->cameraPosition = pos;
}

Lighting::Lighting() {
    directionalLightSource = new DirectionalLightSource(
            glm::vec3(-0.2f, -1.0f, -0.3f),
            glm::vec3(0.05f, 0.05f, 0.05f),
            glm::vec3(0.4f, 0.4f, 0.4f),
            glm::vec3(0.5f, 0.5f, 0.5f)
    );
}

Lighting::~Lighting() {
    delete directionalLightSource;
}

void Lighting::bind(Shader *shader) const {
    shader->setVec3("cameraPos", cameraPosition);
    directionalLightSource->bind(shader, 0);
}
