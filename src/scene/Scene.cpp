#include "Scene.h"
#include "../utils/FileUtils.h"
#include "../utils/ModelLoader.h"
#include "glm/ext/matrix_transform.hpp"
#include "../parser/FileMazeParser.h"
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
    renderMaze();
}

void Scene::renderMaze() {
    ModelLoader* modelLoader = ModelLoader::getInstance();

    std::vector<Mesh*> wallMeshes = modelLoader->loadMeshes("objects/cube/cube.obj");
    std::vector<Mesh*> floorMeshes = modelLoader->loadMeshes("objects/floor/floor.obj");
    Shader shader = Shader("shaders/wall.vs", "shaders/wall.fs");
    MazeParser *mazeParser = new FileMazeParser("maze/maze.txt");

    std::vector<glm::mat4> floorMatrices = {};
    std::vector<glm::mat4> wallMatrices = {};
    for(int i = 0; i < mazeParser->getMaze().size(); i++) {
        for(int j = 0; j < mazeParser->getMaze()[i].size(); j++) {
            if (mazeParser->getMaze()[i][j] == PositionEnum::WALL) {
                wallMatrices.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(i, 0.0f, j)));
            }

            floorMatrices.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(i, -1.0f, j)));
        }
    }

    addObject(new Model(wallMatrices, shader, wallMeshes));
    addObject(new Model(floorMatrices, shader, floorMeshes));
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
