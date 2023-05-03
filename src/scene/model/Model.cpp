#include "Model.h"
#include <iostream>

void Model::render(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix) {
    shader->bind();

    Lighting::getInstance()->bind(shader);

    // the model matrix is only needed if there is more than one model
    if (!isInstanced()) {
        shader->setMat4("model", modelMatrices[0]);
    }

    shader->setMat4("view", viewMatrix);
    shader->setMat4("projection", projectionMatrix);

    for (Mesh *mesh: meshes) {
        mesh->render(*shader);
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

Model::Model(const glm::mat4 modelMatrix, Shader *shader, const std::vector<Mesh *> &meshes) : Model(
        std::vector<glm::mat4>{modelMatrix}, shader, meshes) {}

Model::Model(const std::vector<glm::mat4> &modelMatrices, Shader *shader, const std::vector<Mesh *> &meshes)
        : modelMatrices(modelMatrices), shader(shader), meshes(meshes) {
    if (modelMatrices.size() > 1) {
        setupInstancing();
    }
}

Model::~Model() {
    for (Mesh *mesh: meshes) {
        delete mesh;
    }

    for(PointLight *lightSource: lightSources) {
        delete lightSource;
    }

    if (isInstanced()) {
        glDeleteBuffers(1, &buffer);
    }


    delete shader;

    std::cout << "Model destroyed" << std::endl;
}

std::vector<glm::mat4> Model::getModelMatrices() const {
    return modelMatrices;
}

std::vector<AxisAlignedBB> Model::getBoundingBoxes(const glm::mat4 &modelMatrix) const {
    std::vector<AxisAlignedBB> boundingBoxes = {};
    boundingBoxes.reserve(meshes.size());

    for (Mesh *mesh: meshes) {
        boundingBoxes.push_back(mesh->getBoundingBox().transform(modelMatrix));
    }

    return boundingBoxes;
}

bool Model::isInstanced() const {
    return modelMatrices.size() > 1;
}

void Model::setModelMatrix(const glm::mat4 &modelMatrix) {
    if (isInstanced()) {
        throw std::runtime_error("Cannot set model matrix of instanced model");
    }

    modelMatrices[0] = modelMatrix;
}

void Model::setLightSource(const PointLight &source) {
    auto lighting = Lighting::getInstance();
    for (auto model: modelMatrices) {
        auto clonedSource = new PointLight(source);
        clonedSource->setPosition(glm::vec3(model[3]));
        lighting->addPointLight(clonedSource);

        lightSources.push_back(clonedSource);
    }
}
