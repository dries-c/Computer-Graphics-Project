#ifndef OPENGL_PROJECT_MODEL_H
#define OPENGL_PROJECT_MODEL_H

#include <vector>
#include "glm/mat4x4.hpp"
#include "shader/Shader.h"
#include "../mesh/Mesh.h"

class Model {
private:
    void setupInstancing();

protected:
    std::vector<Mesh *> meshes;
    std::vector<glm::mat4> modelMatrices;
    const Shader *shader;
    unsigned int buffer{};

public:
    virtual ~Model();

    Model(glm::mat4 modelMatrix, const Shader *shader, const std::vector<Mesh *> &meshes);

    Model(const std::vector<glm::mat4> &modelMatrices, const Shader *shader, const std::vector<Mesh *> &meshes);

    virtual void render(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix);

    [[nodiscard]] std::vector<AxisAlignedBB> getBoundingBoxes(const glm::mat4 &modelMatrix) const;

    [[nodiscard]] std::vector<glm::mat4> getModelMatrices() const;

    [[nodiscard]] bool isInstanced() const;

    void setModelMatrix(const glm::mat4 &modelMatrix);
};


#endif
