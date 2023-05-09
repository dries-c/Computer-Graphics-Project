#ifndef OPENGL_PROJECT_SCENE_H
#define OPENGL_PROJECT_SCENE_H

#include "model/Model.h"
#include "skybox/Skybox.h"
#include <vector>
#include "../utils/Structs.h"
#include "../utils/Interactable.h"
#include "../entity/Entity.h"
#include "lighting/Lighting.h"
#include "../utils/PathFindingAlgorithm.h"
#include "../entity/Ghost.h"

class Scene {
public:
    Scene();

    ~Scene();

    void render(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix);
    void doPhysics(float deltaTime, std::vector<AxisAlignedBB> boundingBoxes, glm::vec3 cameraPosition);

    std::vector<AxisAlignedBB> getBoundingBoxes();
    std::vector<Interactable *> getInteractables();
    std::vector<Ghost *> getGhosts();

private:
    std::vector<Model *> objects;
    std::vector<Entity *> entities;
    Skybox *skybox;
    PathFindingAlgorithm *pathFinding;
    float lastPathUpdate = 0.0f;

    void addObject(Model *object);
    void addEntity(Entity *entity);
    void setupMaze();
    void setupSkybox();
};

#endif
