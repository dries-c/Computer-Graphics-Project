#include "Lighting.h"
#include "GLFW/glfw3.h"

#define M_PI   3.14159265358979323846264338327950288

Lighting *Lighting::instance = nullptr;

Lighting::Lighting() {
    directionalLightSource = new DirectionalLight(
            glm::vec3(1.0f, 0.5f, 1.0f),
            glm::vec3(0.0f),
            glm::vec3(0.0f),
            glm::vec3(0.0f),
            glm::vec3(1.0f)
    );
    spotLightSource = new SpotLight(
            glm::vec3(0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            Lighting::rgbToVec3(255, 214, 170),
            cos(12.5f * M_PI / 180.0f),
            cos(25.0f * M_PI / 180.0f),
            1.0f,
            0.09f,
            0.032f
    );
}

void Lighting::toggleSpotlight() {
    if (glfwGetTime() - spotLightToggleTime > SPOTLIGHT_TOGGLE_DELAY) {
        spotLightEnabled = !spotLightEnabled;
        spotLightToggleTime = glfwGetTime();
    }
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
    shader->setBool("spotLightEnabled", spotLightEnabled);

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

glm::vec3 Lighting::rgbToVec3(int r, int g, int b) {
    return {
            std::clamp((float) r / 255.0f, 0.0f, 1.0f),
            std::clamp((float) g / 255.0f, 0.0f, 1.0f),
            std::clamp((float) b / 255.0f, 0.0f, 1.0f)
    };
}
