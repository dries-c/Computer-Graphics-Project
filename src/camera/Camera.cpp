#include "Camera.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "GLFW/glfw3.h"
#include "../sound/SoundProvider.h"
#include "../sound/Sound.h"
#include <algorithm>
#include <iostream>
#include <cfloat>

Camera *Camera::instance = nullptr;

Camera *Camera::getInstance(glm::vec3 position, glm::vec3 up, float yaw, float pitch) {
    if (instance == nullptr) {
        instance = new Camera(position, up, yaw, pitch);
    }

    return instance;
}

void Camera::processKeyboard(Input direction) {
    float speed = hasGravity ? SPEED : FREE_CAM_SPEED;

    switch (direction) {
        case Input::INPUT_FORWARD: {
            velocity.z += front.z * speed;
            velocity.x += front.x * speed;
        }
            break;
        case Input::INPUT_BACKWARD: {
            velocity.z -= front.z * speed;
            velocity.x -= front.x * speed;
        }
            break;
        case Input::INPUT_LEFT: {
            velocity -= right * speed;
        }
            break;
        case Input::INPUT_RIGHT: {
            velocity += right * speed;
        }
            break;
        case Input::INPUT_JUMP: {
            if (hasGravity) {
                jump();
            } else {
                position.y += up.y * speed / 20;
            }
        }
            break;
        case Input::INPUT_DOWN: {
            if (!hasGravity) {
                position.y -= up.y * speed / 20;
            }
        }
            break;

    }
}

void Camera::processKeyboard(FreeCamControls control) {
    switch (control) {
        case FreeCamControls::TOGGLE_FREECAM:
            if (glfwGetTime() - freeCamToggleTime > FREE_CAM_TOGGLE_DELAY) {
                setHasGravity(!hasGravity);
                //stop falling or jumping
                velocity.y = 0;

                freeCamToggleTime = glfwGetTime();
            }
            break;
        case FreeCamControls::FASTER_FREECAM:
            if (!hasGravity) {
                //Add speed to free camera
                FREE_CAM_SPEED += 0.1f;
            }
            break;
        case FreeCamControls::SLOWER_FREECAM:
            if (!hasGravity) {
                //Subtract speed from free camera make sure it's at least 0.05
                FREE_CAM_SPEED -= 0.1f;
                FREE_CAM_SPEED = fmax(FREE_CAM_SPEED, 0.5f);
            }
            break;
        case FreeCamControls::RESET_SPEED_FREECAM:
            if (!hasGravity) {
                //Set speed to default
                FREE_CAM_SPEED = 3.0f;
            }
            break;
    }
}

void Camera::processMouseScroll(float yoffset) {
    fov = std::clamp(fov - yoffset, 1.0f, 45.0f);
}

void Camera::processMouseMovement(float xoffset, float yoffset) {
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw = fmod(yaw + xoffset, 360.0f);
    pitch = std::clamp(pitch + yoffset, -89.0f, 89.0f);

    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    glm::vec3 newFront = glm::vec3(
            cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
            sin(glm::radians(pitch)),
            sin(glm::radians(yaw)) * cos(glm::radians(pitch))
    );

    front = glm::normalize(newFront);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));

    SoundProvider::getInstance()->updateCameraPosition(position, front, up);
    Lighting::getInstance()->updateCameraPosition(position, front);
}

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Entity(
        position,
        AxisAlignedBB(-0.1f, -0.8f, -0.1f, 0.1f, 0.8f, 0.1f),
        yaw,
        pitch
) {

    this->up = up;
    this->worldUp = up;
    this->sensitivity = 0.1f;
    this->fov = 45.0f;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

std::string Camera::toString() const {
    return "Camera: " + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " +
           std::to_string(position.z) + ", YAW: " + std::to_string(yaw) + ", PITCH:" + std::to_string(pitch);
}

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(fov), (float) width / (float) height, 0.1f, 100.0f);
}

void Camera::setWindowDimensions(int width, int height) {
    this->width = width;
    this->height = height;
}

std::pair<Interactable*, float> Camera::rayCast(const std::vector<Interactable *> &interactables, const std::vector<AxisAlignedBB> &colliders) const {
    float closestDistance = FLT_MAX;
    Interactable *closestInteractable = nullptr;

    for (const auto interactable: interactables) {
        for (const auto &bb: interactable->getBoundingBoxes()) {
            float distance = bb.getIntersectionDistance(position, front);
            if (distance < 0) {
                continue;// No intersection
            }

            if (distance < closestDistance) {
                closestDistance = distance;
                closestInteractable = interactable;
            }
        }
    }

    for (const auto &bb: colliders) {
        float distance = bb.getIntersectionDistance(position, front);
        if (distance < 0) {
            continue;// No intersection
        }

        if (distance < closestDistance) {
            return {nullptr, -1}; // We hit a collider, so we can't interact with anything
        }
    }

    return {closestInteractable, closestDistance};
}

void Camera::interact(const std::vector<Interactable *> &interactables, const std::vector<AxisAlignedBB> &colliders) {
    auto[closestModel, distance] = rayCast(interactables, colliders);

    if (closestModel != nullptr && distance < MAX_INTERACT_DISTANCE) {
        closestModel->onInteract(position, distance);
    }
}

void Camera::doPhysics(float deltaTime, const std::vector<AxisAlignedBB> &colliders) {
    bool isMoving = velocity.x > 0 || velocity.z > 0;

    Entity::doPhysics(deltaTime, colliders);
    updateCameraVectors();

    if (onGround) {
        if (wasOnGround) {
            if (isMoving && (walkSound == nullptr || !walkSound->isPlaying())) {
                delete walkSound;
                walkSound = new Sound(Sound::getRandomSound("walk/gravel.ogg", 4), position, 0.2f);
                walkSound->play();
            }
        } else {
            Sound sound = Sound("fall.ogg", position, 0.5f);
            sound.play();
        }
    }

}

void Camera::setPosition(glm::vec3 position) {
    Entity::setPosition(position);
    updateCameraVectors();
}

void Camera::attack(const std::vector<Interactable *> &interactables, const std::vector<AxisAlignedBB> &colliders) {
    auto[closestModel, distance] = rayCast(interactables, colliders);

    if (closestModel != nullptr && distance < MAX_INTERACT_DISTANCE) {
        closestModel->onAttack(position, distance);
    }
}
