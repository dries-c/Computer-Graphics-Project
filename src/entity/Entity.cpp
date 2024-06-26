#include "Entity.h"
#include "glm/ext/matrix_transform.hpp"

void Entity::setPosition(glm::vec3 pos) {
    this->position = pos;

    if (model != nullptr) {
        model->setModelMatrix(getModelMatrix());
    }
}

void Entity::doPhysics(float deltaTime, const std::vector<AxisAlignedBB> &colliders) {
    if (hasGravity) {
        velocity.y -= GRAVITY * deltaTime;
    }

    position += velocity * deltaTime;

    wasOnGround = onGround;
    onGround = false;
    AxisAlignedBB bb = getBoundingBox();
    for (const auto &collider: colliders) {
        if (collider.intersects(bb)) {
            onCollision(collider);
        }
    }

    setPosition(position);

    if (onGround) {
        velocity.x *= 0.8f;
        velocity.z *= 0.8f;

        if (velocity.y < 0) {
            velocity.y = 0;
        }
    }
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

void Entity::setModel(Model *newModel) {
    if (newModel->isInstanced()) {
        throw std::runtime_error("Cannot set instanced model to entity");
    }

    this->model = newModel;
}

Model *Entity::getModel() {
    return model;
}

glm::mat4 Entity::getModelMatrix() const {
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));

    return modelMatrix;
}

std::vector<AxisAlignedBB> Entity::getBoundingBoxes() const {
    if (model == nullptr) {
        return {getBoundingBox()};
    } else {
        return model->getBoundingBoxes(getModelMatrix());
    }
}

void Entity::knockback(glm::vec3 pos, float force) {
    glm::vec3 direction = glm::normalize(position - pos);
    double f = sqrt(direction.x * direction.x + direction.z * direction.z);
    if (f <= 0) {
        return;
    }

    velocity = glm::vec3(
            velocity.x * 0.5f + direction.x * (1.0f / f) * force,
            fmin(velocity.y * 0.5f + force, JUMP_SPEED / 1.25f),
            velocity.z * 0.5f + direction.z * (1.0f / f) * force
    );
}
