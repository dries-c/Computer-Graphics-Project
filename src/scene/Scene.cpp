#include "Scene.h"
#include "../utils/ModelLoader.h"
#include "glm/ext/matrix_transform.hpp"
#include "../parser/FileMazeParser.h"
#include "model/InteractableModel.h"

#include <iostream>

void Scene::render(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix, float deltaTime) {
    for (Entity *entity: entities) {
        if (auto *interactable = dynamic_cast<Interactable *>(entity)) {
            if (!interactable->isAlive()) {
                continue;
            }
        }

        entity->doPhysics(deltaTime, getBoundingBoxes());

        auto model = entity->getModel();
        if (model != nullptr) {
            model->render(viewMatrix, projectionMatrix);
        }
    }
    for (Model *object: objects) {
        if (auto *interactable = dynamic_cast<Interactable *>(object)) {
            if (!interactable->isAlive()) {
                continue;
            }
        }

        object->render(viewMatrix, projectionMatrix);
    }
    skybox->render(viewMatrix, projectionMatrix);
}

void Scene::removeObject(Model *object) {
    objects.erase(std::remove(objects.begin(), objects.end(), object), objects.end());
    delete object;
}

void Scene::addObject(Model *object) {
    objects.push_back(object);
}

Scene::Scene() {
    setupSkybox();
    setupMaze();
}

void Scene::setupMaze() {
    ModelLoader *modelLoader = ModelLoader::getInstance();

    std::vector<Mesh *> wallMeshes = modelLoader->loadMeshes("objects/cube/stone.obj");
    std::vector<Mesh *> floorMeshes = modelLoader->loadMeshes("objects/grass/grass.obj");
    //new CustomMazeParser(29, 29);
    MazeParser *mazeParser = new FileMazeParser("maze/maze.txt");

    glm::mat4 base = glm::mat4(1.0f);
    std::vector<glm::mat4> floorMatrices = {};
    std::vector<glm::mat4> wallMatrices = {};
    for (int i = 0; i < mazeParser->getMaze().size(); i++) {
        for (int j = 0; j < mazeParser->getMaze()[i].size(); j++) {
            PositionEnum position = mazeParser->getMaze()[i][j];

            if (position == PositionEnum::WALL) {
                wallMatrices.push_back(glm::translate(base, glm::vec3(i, -1.0f, -j)));
            } else if (position == PositionEnum::OBSTACLE) {
                // needs to be an instance, since when the model is removed from the scene, it will be deleted
                auto obstacleMesh = modelLoader->loadMeshes("objects/obstacle/obstacle.obj");
                addObject(new InteractableModel(glm::translate(base, glm::vec3(i, -1.0f, -j)),
                                                new Shader("shaders/singular.vs", "shaders/singular.fs"),
                                                obstacleMesh));
            }

            floorMatrices.push_back(glm::translate(base, glm::vec3(i, -1.0f, -j)));
        }
    }

    addObject(new Model(wallMatrices, new Shader("shaders/instanced.vs", "shaders/instanced.fs"), wallMeshes));
    addObject(new Model(floorMatrices, new Shader("shaders/instanced.vs", "shaders/instanced.fs"), floorMeshes));
}

Scene::~Scene() {
    for (Model *object: objects) {
        delete object;
    }

    delete skybox;

    std::cout << "Scene destroyed" << std::endl;
}

void Scene::setupSkybox() {
    skybox = new Skybox({
                                "right.jpg",
                                "left.jpg",
                                "top.jpg",
                                "bottom.jpg",
                                "front.jpg",
                                "back.jpg"
                        });
}

std::vector<AxisAlignedBB> Scene::getBoundingBoxes() {
    std::vector<AxisAlignedBB> boundingBoxes = {};

    for (Model *object: objects) {
        if (auto *interactable = dynamic_cast<Interactable *>(object)) {
            if (!interactable->isAlive()) {
                continue;
            }
        }

        for (const auto &modelMatrix: object->getModelMatrices()) {
            std::vector<AxisAlignedBB> objectBoundingBoxes = object->getBoundingBoxes(modelMatrix);
            boundingBoxes.insert(boundingBoxes.end(), objectBoundingBoxes.begin(), objectBoundingBoxes.end());
        }
    }

    return boundingBoxes;
}

void Scene::addEntity(Entity *entity) {
    entities.push_back(entity);
}

std::vector<Interactable *> Scene::getInteractables() {
    std::vector<Interactable *> interactables = {};

    for (Entity *entity: entities) {
        if (auto interactable = dynamic_cast<Interactable *>(entity)) {
            if (interactable->isAlive()) {
                interactables.push_back(interactable);
            }
        }
    }

    for (Model *object: objects) {
        if (auto interactable = dynamic_cast<Interactable *>(object)) {
            if (interactable->isAlive()) {
                interactables.push_back(interactable);
            }
        }
    }

    return interactables;
}
