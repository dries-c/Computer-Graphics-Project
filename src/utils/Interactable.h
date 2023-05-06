#ifndef OPENGL_PROJECT_INTERACTABLE_H
#define OPENGL_PROJECT_INTERACTABLE_H

#include <iostream>
#include "AxisAlignedBB.h"
#include "../sound/Sound.h"


class Interactable {
private:
    bool alive = true;
public:
    virtual void onAttack() = 0;
    virtual void onInteract() = 0;
    [[nodiscard]] bool isAlive() const { return alive; }
    void kill() {
        alive = false;
        Sound sound = Sound("stone4.ogg");
        sound.play();
    }
    [[nodiscard]] virtual std::vector<AxisAlignedBB> getBoundingBoxes() const = 0;
    [[nodiscard]] virtual glm::mat4 getModelMatrix() const = 0;
};


#endif
