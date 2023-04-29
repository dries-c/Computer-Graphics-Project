#include "Entity.h"

void Entity::setPosition(glm::vec3 pos) {
    this->position = pos;
}

void Entity::update(float deltaTime, const std::vector<AxisAlignedBB> &colliders) {
    if (hasGravity) {
        velocity.y -= GRAVITY * deltaTime;
    }

    position += velocity * deltaTime;

    onGround = false;
    AxisAlignedBB bb = getBoundingBox();
    for (const auto &collider: colliders) {
        if (collider.intersects(bb)) {
            onCollision(collider);
        }
    }

    setPosition(position);

    velocity.x = 0;
    if (onGround && velocity.y < 0) {
        velocity.y = 0;
    }
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
    return onGround;
}

void Entity::onCollision(const AxisAlignedBB &collider) {
    AxisAlignedBB bb = getBoundingBox();
    glm::vec3 halfSize = bb.getSize() / 2.0f;
    Face face = bb.getIntersection(collider);
    switch (face) {
        case Face::FACE_LEFT:
            position.x = collider.max.x + halfSize.x;
            break;
        case Face::FACE_RIGHT:
            position.x = collider.min.x - halfSize.x;
            break;
        case Face::FACE_TOP:
            position.y = collider.max.y + halfSize.y;
            onGround = true;
            break;
        case Face::FACE_BOTTOM:
            position.y = collider.min.y - halfSize.y;
            break;
        case Face::FACE_FRONT:
            position.z = collider.min.z - halfSize.z;
            break;
        case Face::FACE_BACK:
            position.z = collider.max.z + halfSize.z;
            break;
    }
}
