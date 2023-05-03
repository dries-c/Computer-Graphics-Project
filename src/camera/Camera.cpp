#include "Camera.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "GLFW/glfw3.h"
#include <algorithm>
#include <iostream>
#include <limits>
#include <float.h>

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
                position.y += FREE_CAM_SPEED / 500;
            }
        }
            break;
        case Input::INPUT_DOWN: {
            if (!hasGravity) {
                position.y -= FREE_CAM_SPEED / 500;
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
                freeCamToggleTime = glfwGetTime();
            }
            break;
        case FreeCamControls::FASTER_FREECAM:
            if (!hasGravity) {
                //Add speed to free camera
                FREE_CAM_SPEED += 0.01f;
            }
            break;
        case FreeCamControls::SLOWER_FREECAM:
            if (!hasGravity) {
                //Subtract speed from free camera make sure it's at least 0.05
                FREE_CAM_SPEED -= 0.01f;
                FREE_CAM_SPEED = max(FREE_CAM_SPEED, 0.05f);
            }
            break;
        case FreeCamControls::RESET_SPEED_FREECAM:
            if (!hasGravity) {
                //Set speed to default
                FREE_CAM_SPEED = 2.0f;
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

Interactable *Camera::rayCast(const std::vector<Interactable *> &interactables) {
    float closestDistance = FLT_MAX;
    Interactable *closestInteractable = nullptr;

    for (const auto interactable: interactables) {
        for (const auto &bb: interactable->getBoundingBoxes()) {
            float distance = bb.getIntersectionDistance(position, front);
            if (distance == -1) {
                continue;// No intersection
            }

            if (distance < closestDistance) {
                closestDistance = distance;
                closestInteractable = interactable;
            }
        }
    }

    return closestInteractable;
}

void Camera::interact(const std::vector<Interactable *> &interactables) {
    auto closestModel = rayCast(interactables);

    if (closestModel != nullptr) {
        closestModel->onInteract();
    }
}

void Camera::doPhysics(float deltaTime, const std::vector<AxisAlignedBB> &colliders) {
    Entity::doPhysics(deltaTime, colliders);
    updateCameraVectors();
}

void Camera::setPosition(glm::vec3 position) {
    Entity::setPosition(position);
    updateCameraVectors();
}

void Camera::attack(const std::vector<Interactable *> &interactables) {
    auto closestModel = rayCast(interactables);

    if (closestModel != nullptr) {
        closestModel->onAttack();
    }
}

glm::vec3 Camera::getFront() const {
    return front;
}
