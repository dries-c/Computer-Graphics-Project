#ifndef OPENGL_PROJECT_SCENE_H
#define OPENGL_PROJECT_SCENE_H

#include "model/Model.h"
#include "skybox/Skybox.h"
#include <vector>
#include "../utils/Structs.h"
#include "../utils/Interactable.h"
#include "../entity/Entity.h"
#include "lighting/Lighting.h"

class Scene {
public:
    Scene();

    ~Scene();

    void render(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix, float deltaTime);

    std::vector<AxisAlignedBB> getBoundingBoxes();
    std::vector<Interactable *> getInteractables();

private:
    std::vector<Model *> objects;
    std::vector<Entity *> entities;
    Lighting *lighting;
    Skybox *skybox;

    void addObject(Model *object);
    void addEntity(Entity *entity);
    void setupMaze();
    void setupSkybox();
};

#endif
