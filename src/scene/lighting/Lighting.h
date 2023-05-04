#ifndef OPENGL_PROJECT_LIGHTING_H
#define OPENGL_PROJECT_LIGHTING_H

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include <vector>

class Lighting {
private:
    static Lighting* instance;
    Lighting();

    DirectionalLight *directionalLightSource;
    SpotLight *spotLightSource;
    glm::vec3 cameraPosition = glm::vec3(0.0f);
    std::vector<PointLight *> pointLights;
public:
    ~Lighting();

    void addPointLight(PointLight *pointLight);
    void updateCameraPosition(glm::vec3 pos, glm::vec3 direction);

    void bind(Shader *shader) const;
    static Lighting *getInstance();
};


#endif
