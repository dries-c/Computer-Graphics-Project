#include "Scene.h"
#include "../utils/FileUtils.h"
#include <iostream>

void Scene::render(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix) {
    for (Model *object: objects) {
        object->render(viewMatrix, projectionMatrix);
    }
}

void Scene::addObject(Model *object) {
    objects.push_back(object);
}

Scene::Scene() {
    Shader shader = Shader("shaders/wall.vs", "shaders/wall.fs");

    std::vector<Vertex> vertices = {};
    bindToVector(vertices, glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 1.0f));
    bindToVector(vertices, glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 0.0f));
    bindToVector(vertices, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f));
    bindToVector(vertices, glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f));

    bindToVector(vertices, glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 1.0f));
    bindToVector(vertices, glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f));
    bindToVector(vertices, glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 0.0f));
    bindToVector(vertices, glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 1.0f));

    std::vector<unsigned int> indices = {
            0, 1, 2,
            2, 3, 0,

            1, 5, 6,
            6, 2, 1,

            7, 6, 5,
            5, 4, 7,

            4, 0, 3,
            3, 7, 4,

            4, 5, 1,
            1, 0, 4,

            3, 2, 6,
            6, 7, 3
    };

    std::vector<Texture> textures = {
            Texture("textures/container.png")
    };

    auto *model = new Model(glm::mat4(1.0f), shader, {
            new Mesh(vertices, indices, textures)
    });

    addObject(model);
}

Scene::~Scene() {
    for (Model *object: objects) {
        delete object;
    }

    std::cout << "Scene destroyed" << std::endl;
}

void Scene::bindToVector(std::vector<Vertex> &vertices, glm::vec3 vector, glm::vec2 texCoords) {
    Vertex vertex = Vertex();
    vertex.position = vector;
    vertex.texCoords = texCoords;
    vertices.push_back(vertex);
}
