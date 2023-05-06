#ifndef OPENGL_PROJECT_OBSTACLE_H
#define OPENGL_PROJECT_OBSTACLE_H

#include "InteractableModel.h"

class Obstacle : public InteractableModel {
public:
    Obstacle(glm::mat4 modelMatrix, Shader *shader, const std::vector<Mesh *> &meshes) : InteractableModel(modelMatrix, shader, meshes) {};
    void onInteract() override;
    void onAttack() override;
};

#endif
