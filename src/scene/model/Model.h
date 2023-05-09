#ifndef OPENGL_PROJECT_MODEL_H
#define OPENGL_PROJECT_MODEL_H

#include <vector>
#include "glm/mat4x4.hpp"
#include "shader/Shader.h"
#include "../mesh/Mesh.h"
#include "../lighting/Lighting.h"

class Model {
private:
    void setupInstancing();
    [[nodiscard]] bool isHit() const;

protected:
    std::vector<Mesh *> meshes;
    std::vector<glm::mat4> modelMatrices;
    Shader *shader;
    std::vector<PointLight *> lightSources;
    unsigned int buffer{};
    bool collision;
    double lastHitTime = 0;

public:
    virtual ~Model();

    Model(glm::mat4 modelMatrix, Shader *shader, const std::vector<Mesh *> &meshes, bool collision = true);

    Model(const std::vector<glm::mat4> &modelMatrices, Shader *shader, const std::vector<Mesh *> &meshes, bool collision = true);

    virtual void render(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix);

    void setLightSource(const PointLight& lightSource);

    [[nodiscard]] std::vector<AxisAlignedBB> getBoundingBoxes(const glm::mat4 &modelMatrix) const;

    [[nodiscard]] std::vector<glm::mat4> getModelMatrices() const;

    [[nodiscard]] bool isInstanced() const;

    void setModelMatrix(const glm::mat4 &modelMatrix);

    void onHit();
};


#endif
