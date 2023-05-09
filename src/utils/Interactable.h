#ifndef OPENGL_PROJECT_INTERACTABLE_H
#define OPENGL_PROJECT_INTERACTABLE_H

#include <iostream>
#include "AxisAlignedBB.h"
#include "../sound/Sound.h"

#define MAX_INTERACT_DISTANCE 3.5f

class Interactable {
protected:
    bool alive = true;
public:
    virtual void onAttack(glm::vec3 position, float distance) = 0;
    virtual void onInteract(glm::vec3 position, float distance) = 0;
    [[nodiscard]] bool isAlive() const { return alive; }
    void kill(){ alive = false; }
    [[nodiscard]] virtual std::vector<AxisAlignedBB> getBoundingBoxes() const = 0;
    [[nodiscard]] virtual glm::mat4 getModelMatrix() const = 0;
};


#endif
