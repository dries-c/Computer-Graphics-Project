#include "Entity.h"

void Entity::setPosition(glm::vec3 pos) {
    this->position = pos;
}

void Entity::update(float deltaTime) {
    if (hasGravity) {
        velocity.y -= GRAVITY * deltaTime;
    }

    oldPosition = position;
    position += velocity * deltaTime;
    if (isOnGround()) {
        position.y = MIN_Y;
        velocity.y = 0;
    }

    velocity.x = 0;
    velocity.z = 0;
}

AxisAlignedBB Entity::getBoundingBox() const {
    return boundingBox.offset(position);
}

Entity::Entity(glm::vec3 position, AxisAlignedBB boundingBox, float yaw, float pitch) : position(position),
                                                                                     boundingBox(boundingBox),
                                                                                     yaw(yaw),
                                                                                     pitch(pitch) {}

glm::vec3 Entity::getPosition() const {
    return position;
}

void Entity::setHasGravity(bool gravity) {
    this->hasGravity = gravity;
}

void Entity::jump() {
    if (hasGravity && isOnGround()) {
        velocity.y = JUMP_SPEED;
    }
}

bool Entity::isOnGround() const {
    return position.y <= MIN_Y;
}

void Entity::revertPosition() {
    setPosition(oldPosition);
}

void Entity::onCollision(AxisAlignedBB &bb) {
    revertPosition();
}
