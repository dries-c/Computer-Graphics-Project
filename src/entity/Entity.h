#ifndef OPENGL_PROJECT_ENTITY_H
#define OPENGL_PROJECT_ENTITY_H

#include "glm/vec3.hpp"
#include "../utils/AxisAlignedBB.h"
#include "../scene/model/Model.h"
#include "../utils/Interactable.h"

#define GRAVITY 9.81f
#define JUMP_SPEED 3.0f

class Entity {
protected:
    glm::vec3 position{};
    glm::vec3 oldPosition{};
    glm::vec3 velocity{};
    AxisAlignedBB boundingBox;

    bool onGround = false;
    bool wasOnGround = false;
    bool hasGravity = false;
    Model *model = nullptr;
    float yaw;
    float pitch;
public:
    explicit Entity(glm::vec3 position, AxisAlignedBB boundingBox, float yaw, float pitch);

    void setModel(Model *model);

    Model *getModel();

    virtual void doPhysics(float deltaTime, const std::vector<AxisAlignedBB> &colliders);

    [[nodiscard]] AxisAlignedBB getBoundingBox() const;

    virtual void setPosition(glm::vec3 position);

    [[nodiscard]] glm::vec3 getPosition() const;

    void setHasGravity(bool hasGravity);

    void jump();

    [[nodiscard]] bool isOnGround() const;

    void onCollision(const AxisAlignedBB &collider);

    [[nodiscard]] std::vector<AxisAlignedBB> getBoundingBoxes() const;
    [[nodiscard]] glm::mat4 getModelMatrix() const;
};


#endif
