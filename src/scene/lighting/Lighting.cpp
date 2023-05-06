#include "Lighting.h"
#define _USE_MATH_DEFINES
#include <cmath>
#define M_PI   3.14159265358979323846264338327950288

Lighting *Lighting::instance = nullptr;

Lighting::Lighting() {
    directionalLightSource = new DirectionalLight(
            glm::vec3(1.0f, 0.5f, 1.0f),
            glm::vec3(0.0f),
            glm::vec3(0.0f),
            glm::vec3(0.0f)
    );
    spotLightSource = new SpotLight(
            glm::vec3(0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            cos(12.5f * M_PI / 180.0f),
            cos(25.0f * M_PI / 180.0f),
            1.0f,
            0.09f,
            0.032f
   );
}

Lighting::~Lighting() {
    delete directionalLightSource;
}

void Lighting::bind(Shader *shader) const {
    shader->setVec3("cameraPos", cameraPosition);
    directionalLightSource->bind(shader, 0);

    for (int i = 0; i < pointLights.size(); i++) {
        pointLights[i]->bind(shader, i);
    }
    shader->setInt("pointLightCount", pointLights.size());

    spotLightSource->bind(shader, 0);
}

void Lighting::addPointLight(PointLight *pointLight) {
    pointLights.push_back(pointLight);
}

void Lighting::updateCameraPosition(glm::vec3 pos, glm::vec3 direction) {
    this->cameraPosition = pos;

    spotLightSource->updatePosition(pos, direction);
}

Lighting *Lighting::getInstance() {
    if (instance == nullptr) {
        instance = new Lighting();
    }

    return instance;
}
