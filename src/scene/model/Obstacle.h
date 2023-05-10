#ifndef OPENGL_PROJECT_OBSTACLE_H
#define OPENGL_PROJECT_OBSTACLE_H

#include "InteractableModel.h"
#include "../../utils/ModelLoader.h"
#include "../../utils/PathFindingAlgorithm.h"

class Obstacle : public InteractableModel {
private:
    PathFindingAlgorithm &pathfindingAlgorithm;
public:
    Obstacle(glm::mat4 modelMatrix, PathFindingAlgorithm& pathfindingAlgorithm)
            : InteractableModel(
            modelMatrix,
            new Shader("shaders/singular.vs", "shaders/lighting.fs"),
            ModelLoader::getInstance()->loadMeshes("objects/obstacle/obstacle.obj"),
            true
    ), pathfindingAlgorithm(pathfindingAlgorithm) {};

    void onInteract(glm::vec3 position, float distance) override;

    void onAttack(glm::vec3 position, float distance) override;
};

#endif
