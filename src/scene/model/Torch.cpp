#include "Torch.h"
#include "GLFW/glfw3.h"
#include "../../sound/Sound.h"

Torch::Torch(const std::vector<glm::mat4> &modelMatrices) : Model(
        modelMatrices,
        new Shader("shaders/instanced.vs", "shaders/noLighting.fs"),
        ModelLoader::getInstance()->loadMeshes("objects/torch/torch.obj"),
        false
) {
    setLightSource(PointLight(
            glm::vec3(0.01f, 0.01f, 0.01f),
            glm::vec3(0.5f, 0.5f, 0.5f),
            glm::vec3(0.10f, 0.10f, 0.10f),
            Lighting::rgbToVec3(255, 197, 143),
            0.1f,
            0.09f,
            0.032f
    ));
}

void Torch::render(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix) {
    Model::render(viewMatrix, projectionMatrix);

    for (int i = 0; i < modelMatrices.size(); i++) {
        if (sounds.size() <= i) {
            auto *sound = new Sound(Sound::getRandomSound("torch/crackle.ogg", 6), glm::vec3(modelMatrices[i][3]));
            sound->play();

            sounds.push_back(sound);
        } else if (!sounds[i]->isPlaying()) {
            delete sounds[i];

            sounds[i] = new Sound(Sound::getRandomSound("torch/crackle.ogg", 6), glm::vec3(modelMatrices[i][3]));
            sounds[i]->play();
        }
    }
}
