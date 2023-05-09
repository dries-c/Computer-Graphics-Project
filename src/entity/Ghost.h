#ifndef OPENGL_PROJECT_GHOST_H
#define OPENGL_PROJECT_GHOST_H


#include "AIEntity.h"

class Ghost : public AIEntity, public Interactable {
private:
    int health;
public:
    void onAttack(glm::vec3 position, float distance) override;
    void onInteract(glm::vec3 position, float distance) override;

    Ghost(glm::vec3 position);
    [[nodiscard]] std::vector<AxisAlignedBB> getBoundingBoxes() const override { return Entity::getBoundingBoxes(); }
    [[nodiscard]] glm::mat4 getModelMatrix() const override { return Entity::getModelMatrix(); }
};


#endif
