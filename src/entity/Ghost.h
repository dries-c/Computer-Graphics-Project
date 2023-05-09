#ifndef OPENGL_PROJECT_GHOST_H
#define OPENGL_PROJECT_GHOST_H


#include "AIEntity.h"
#define SOUND_COOLDOWN 10.0f

class Ghost : public AIEntity, public Interactable {
private:
    int health;
    double lastAttackTime = 0.0f;
    double lastSoundTime = 0.0f;

public:
    void onAttack(glm::vec3 position, float distance) override;
    void onInteract(glm::vec3 position, float distance) override;

    bool canAttack(glm::vec3 position);
    void attack();
    void doPhysics(float deltaTime, const std::vector<AxisAlignedBB> &colliders) override;

    Ghost(glm::vec3 position);
    [[nodiscard]] std::vector<AxisAlignedBB> getBoundingBoxes() const override { return Entity::getBoundingBoxes(); }
    [[nodiscard]] glm::mat4 getModelMatrix() const override { return Entity::getModelMatrix(); }
};


#endif
