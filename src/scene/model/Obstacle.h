#ifndef OPENGL_PROJECT_OBSTACLE_H
#define OPENGL_PROJECT_OBSTACLE_H

#include "InteractableModel.h"

class Obstacle : public InteractableModel {
public:
    Obstacle(glm::mat4 modelMatrix, Shader *shader, const std::vector<Mesh *> &meshes, bool collision = true) : InteractableModel(modelMatrix, shader, meshes, collision) {};
    void onInteract(glm::vec3 position, float distance) override;
    void onAttack(glm::vec3 position, float distance) override;
};

#endif
