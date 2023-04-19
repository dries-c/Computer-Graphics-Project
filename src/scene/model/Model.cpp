#include "Model.h"
#include <iostream>

void Model::render(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix) {
    shader.bind();

    // the model matrix is only needed if there is more than one model
    if (modelMatrices.size() == 1) {
        shader.setMat4("model", modelMatrices[0]);
    }

    shader.setMat4("view", viewMatrix);
    shader.setMat4("projection", projectionMatrix);

    for (Mesh *mesh: meshes) {
        mesh->render(shader);
    }

    Shader::unbind();
}

void Model::setupInstancing() {
    int count = modelMatrices.size();

    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

    for (Mesh *mesh: meshes) {
        mesh->setupInstancing(count);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Model::Model(glm::mat4 modelMatrices, Shader shader, const std::vector<Mesh *> &meshes) : Model(
        std::vector<glm::mat4>{modelMatrices}, shader, meshes) {}

Model::Model(const std::vector<glm::mat4> &modelMatrices, Shader shader, const std::vector<Mesh *> &meshes)
        : modelMatrices(modelMatrices), shader(shader), meshes(meshes) {
    if (modelMatrices.size() > 1) {
        setupInstancing();
    }
}

Model::~Model() {
    for (Mesh *mesh: meshes) {
        delete mesh;
    }
    if (modelMatrices.size() > 1) {
        glDeleteBuffers(1, &buffer);
    }

    std::cout << "Model destroyed" << std::endl;
}
