#include "Scene.h"
#include "../utils/ModelLoader.h"
#include "glm/ext/matrix_transform.hpp"
#include "model/InteractableModel.h"
#include "../parser/RandomMazeParser.h"
#include "model/Obstacle.h"
#include "GLFW/glfw3.h"
#include "../entity/AIEntity.h"
#include "../entity/Ghost.h"
#include "model/Torch.h"
#include <iostream>

#define PATH_FINDING_INTERVAL 0.5f

void Scene::render(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix) {
    for (Entity *entity: entities) {
        if (auto *interactable = dynamic_cast<Interactable *>(entity)) {
            if (!interactable->isAlive()) {
                continue;
            }
        }

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

void Scene::addObject(Model *object) {
    objects.push_back(object);
}

Scene::Scene() {
    setupSkybox();
    setupMaze();
}

void Scene::setupMaze() {
    ModelLoader *modelLoader = ModelLoader::getInstance();

    std::vector<Mesh *> wallMeshes = modelLoader->loadMeshes("objects/wall/wall.obj");
    std::vector<Mesh *> floorMeshes = modelLoader->loadMeshes("objects/floor/floor.obj");

    MazeParser *mazeParser = new RandomMazeParser(29, 29, "maze/maze.txt");
    pathFinding = new PathFindingAlgorithm(mazeParser->getWalkableMaze());

    glm::mat4 base = glm::mat4(1.0f);;
    std::vector<glm::mat4> floorMatrices = {};
    std::vector<glm::mat4> wallMatrices = {};
    std::vector<glm::mat4> torchMatrices = {};
    for (int i = 0; i < mazeParser->getMaze().size(); i++) {
        for (int j = 0; j < mazeParser->getMaze()[i].size(); j++) {
            PositionEnum position = mazeParser->getMaze()[i][j];

            if (position == PositionEnum::LIGHT) {
                glm::mat4 torchPos = glm::rotate(glm::translate(base, glm::vec3(i + 0.5f, 0.125f, j + 0.5f)),
                                                 glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                torchMatrices.push_back(glm::scale(torchPos, glm::vec3(0.3f, 0.3f, 0.3f)));
            }else if(position == PositionEnum::GHOST){
                //dont spawn an entitity on the torch on position 1,1
                if (i != 1 || j != 1) {
                    addEntity(new Ghost(glm::vec3(i + 0.5f, 0.8f, j + 0.5f)));
                }
            } else if (position == PositionEnum::WALL) {
                wallMatrices.push_back(glm::translate(base, glm::vec3(i, 0.0f, j)));
            } else if (position == PositionEnum::GHOST) {
                addEntity(new Ghost(glm::vec3(i + 0.5f, 0.8f, j + 0.5f)));
            } else if (position == PositionEnum::OBSTACLE) {
                addObject(new Obstacle(glm::translate(base, glm::vec3(i, 0.0f, j)), *pathFinding));
            }

            floorMatrices.push_back(glm::translate(base, glm::vec3(i, 0.0f, j)));
        }
    }

    addObject(new Model(wallMatrices, new Shader("shaders/instanced.vs", "shaders/lighting.fs"), wallMeshes));
    addObject(new Model(floorMatrices, new Shader("shaders/instanced.vs", "shaders/lighting.fs"), floorMeshes));
    addObject(new Torch(torchMatrices));

    Sound sound = Sound("startup.ogg");
    sound.play();

    Sound ambient = Sound("ambience.ogg", 4.0f, true);
    ambient.play();
}

Scene::~Scene() {
    for (Model *object: objects) {
        delete object;
    }

    delete skybox;
    delete pathFinding;

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

void Scene::doPhysics(float deltaTime, std::vector<AxisAlignedBB> boundingBoxes, glm::vec3 cameraPosition) {
    bool updatePathFinding = lastPathUpdate + PATH_FINDING_INTERVAL < glfwGetTime();

    for (Entity *entity: entities) {
        if (auto *interactable = dynamic_cast<Interactable *>(entity)) {
            if (!interactable->isAlive()) {
                continue;
            }
        }

        if (updatePathFinding) {
            if (auto aiEntity = dynamic_cast<AIEntity *>(entity)) {
                aiEntity->updatePath(pathFinding, cameraPosition);
            }
        }

        entity->doPhysics(deltaTime, boundingBoxes);
    }

    if (updatePathFinding) {
        lastPathUpdate = glfwGetTime();
    }
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

std::vector<Ghost *> Scene::getGhosts() {
    std::vector<Ghost *> ghosts = {};

    for (Entity *entity: entities) {
        if (auto ghost = dynamic_cast<Ghost *>(entity)) {
            if (!ghost->isAlive()) {
                continue;
            }

            ghosts.push_back(ghost);
        }
    }

    return ghosts;
}
