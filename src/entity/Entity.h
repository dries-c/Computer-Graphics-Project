#ifndef OPENGL_PROJECT_ENTITY_H
#define OPENGL_PROJECT_ENTITY_H

#include "glm/vec3.hpp"
#include "../utils/AxisAlignedBB.h"

#define MIN_Y 0.0f
#define GRAVITY 9.81f
#define JUMP_SPEED 4.0f

class Entity {
protected:
    glm::vec3 position{};
    glm::vec3 oldPosition{};
    glm::vec3 velocity{};
    AxisAlignedBB boundingBox;

    bool hasGravity = true;

    float yaw;
    float pitch;
public:
    explicit Entity(glm::vec3 position, AxisAlignedBB boundingBox, float yaw, float pitch);
    virtual void update(float deltaTime);
    [[nodiscard]] AxisAlignedBB getBoundingBox() const;

    virtual void setPosition(glm::vec3 position);
    virtual void revertPosition();
    [[nodiscard]] glm::vec3 getPosition() const;

    void setHasGravity(bool hasGravity);
    void jump();

    [[nodiscard]] bool isOnGround() const;

    void onCollision(AxisAlignedBB &bb);
};


#endif
