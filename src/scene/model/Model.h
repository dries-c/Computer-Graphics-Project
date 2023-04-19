#ifndef OPENGL_PROJECT_MODEL_H
#define OPENGL_PROJECT_MODEL_H

#include <vector>
#include "glm/mat4x4.hpp"
#include "shader/Shader.h"
#include "../mesh/Mesh.h"

class Model {
private:
    std::vector<Mesh *> meshes;
    std::vector<glm::mat4> modelMatrices;
    Shader shader;
    unsigned int buffer;

public:
    virtual ~Model();
    Model(glm::mat4 modelMatrices, Shader shader, const std::vector<Mesh*> &meshes);
    Model(const std::vector<glm::mat4>& modelMatrices, Shader shader, const std::vector<Mesh*> &meshes);
    virtual void render(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix);
    void setupInstancing();
};


#endif
