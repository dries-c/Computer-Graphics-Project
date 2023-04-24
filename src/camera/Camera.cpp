#include "Camera.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include <algorithm>
#include <iostream>

Camera *Camera::instance = nullptr;

Camera *Camera::getInstance(glm::vec3 position, glm::vec3 up, float yaw, float pitch) {
    if (instance == nullptr) {
        instance = new Camera(position, up, yaw, pitch);
    }

    return instance;
}

void Camera::processKeyboard(Direction direction) {
    switch (direction) {
        case Direction::FORWARD:
            if (freeCamera) {
                velocity.z += front.z * (FREE_CAM_SPEED);
                velocity.x += front.x * (FREE_CAM_SPEED);
            } else {
                velocity.z += front.z * SPEED;
                velocity.x += front.x * SPEED;
            }
            break;
        case Direction::BACKWARD:
            if (freeCamera) {
                velocity.z -= front.z * (FREE_CAM_SPEED);
                velocity.x -= front.x * (FREE_CAM_SPEED);
            } else {
                velocity.z -= front.z * SPEED;
                velocity.x -= front.x * SPEED;
            }
            break;
        case Direction::LEFT:
            if (freeCamera) {
                velocity -= right * (FREE_CAM_SPEED);
            } else {
                velocity -= right * SPEED;
            }
            break;
        case Direction::RIGHT:
            if (freeCamera) {
                velocity += right * (FREE_CAM_SPEED);
            } else {
                velocity += right * SPEED;
            }

            break;
        case Direction::JUMP:
            if (freeCamera) {
                position.y += FREE_CAM_SPEED/500;
            } else if (isOnGround()) {
                velocity.y = JUMP_SPEED;
            }
            break;
        case Direction::DOWN:
            if (freeCamera) {
                position.y -= FREE_CAM_SPEED/500;
            } else {
                velocity -= worldUp * SPEED;
            }
            break;

    }
}
void Camera::processKeyboard(FreeCamControls control) {
    switch (control) {
        case FreeCamControls::TOGGLE_FREECAM:
            freeCamera = !freeCamera;
            break;
        case FreeCamControls::FASTER_FREECAM:
            if (freeCamera) {
                //Add speed to free camera
                FREE_CAM_SPEED += 0.01f;
            }
            break;
        case FreeCamControls::SLOWER_FREECAM:
            if (freeCamera) {
                //Subtract speed from free camera make sure it's at least 0.05
                FREE_CAM_SPEED -= 0.01f;
                FREE_CAM_SPEED = std::max(FREE_CAM_SPEED, 0.05f);
            }
            break;
        case FreeCamControls::RESET_SPEED_FREECAM:
            if (freeCamera) {
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

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) {
    this->position = position;
    this->up = up;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
    this->sensitivity = 0.1f;
    this->fov = 45.0f;
    updateCameraVectors();
}

void Camera::physicsUpdate(float deltaTime) {
    position += velocity * deltaTime;

    if (isOnGround()) {
        velocity.y = 0;
        position.y = MIN_Y;
    } else if (!freeCamera) {
        velocity.y -= GRAVITY * deltaTime;
    }

    velocity.x = 0;
    velocity.z = 0;
}

bool Camera::isOnGround() const {
    return position.y <= MIN_Y;
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