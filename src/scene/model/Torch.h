#ifndef OPENGL_PROJECT_TORCH_H
#define OPENGL_PROJECT_TORCH_H


#include "Model.h"
#include "../../utils/ModelLoader.h"
#include "../../sound/Sound.h"

class Torch : public Model {
private:
    double lastSoundTime = 0;
    std::vector<Sound *> sounds;
public:
    Torch(const std::vector<glm::mat4> &modelMatrices);
    void render(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix) override;
};


#endif
