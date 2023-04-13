#include "Scene.h"
#include "glm/ext/matrix_transform.hpp"
#include "../utils/FileUtils.h"

void Scene::render(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix) {
    for (Model &object : objects) {
        object.render(viewMatrix, projectionMatrix);
    }
}

void Scene::addObject(Model &object) {
    objects.push_back(object);
}

Scene::Scene() {
    Shader shader = Shader("shaders/wall.vs", "shaders/wall.fs");

    std::vector<Vertex> vertices = {};
    bindToVector(vertices, glm::vec3(0.5f, 0.5f, 0.0f), glm::vec2(1.0f, 1.0f));
    bindToVector(vertices, glm::vec3(0.5f, -0.5f, 0.0f), glm::vec2(1.0f, 0.0f));
    bindToVector(vertices, glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(0.0f, 0.0f));
    bindToVector(vertices, glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec2(0.0f, 1.0f));

    std::vector<unsigned int> indices = {
            0, 1, 3,
            1, 2, 3
    };

    std::vector<Texture> textures = {
            Texture("textures/container.png")
    };

    Mesh mesh = Mesh(vertices, indices, textures);
    Model model = Model(glm::mat4(1.0f), shader, {mesh});

    addObject(model);
}

void Scene::bindToVector(std::vector<Vertex> &vertices, glm::vec3 vector, glm::vec2 texCoords) {
    Vertex vertex = Vertex();
    vertex.position = vector;
    vertex.texCoords = texCoords;
    vertices.push_back(vertex);
}
