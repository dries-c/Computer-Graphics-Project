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
    float speed = hasGravity ? SPEED : freeCamSpeed;

    switch (direction) {
        case Input::INPUT_FORWARD: {
            position.z += front.z * speed;
            position.x += front.x * speed;
            didMove = true;
        }
            break;
        case Input::INPUT_BACKWARD: {
            position.z -= front.z * speed;
            position.x -= front.x * speed;
            didMove = true;
        }
            break;
        case Input::INPUT_LEFT: {
            position -= right * speed;
            didMove = true;
        }
            break;
        case Input::INPUT_RIGHT: {
            position += right * speed;
            didMove = true;
        }
            break;
        case Input::INPUT_JUMP: {
            if (hasGravity) {
                jump();
            } else {
                position.y += up.y * speed;
            }
        }
            break;
        case Input::INPUT_DOWN: {
            if (!hasGravity) {
                position.y -= up.y * speed;
            }
        }
            break;

    }
}

void Camera::processKeyboard(FreeCamControls control) {
    switch (control) {
        case FreeCamControls::TOGGLE_FREECAM:
            if (glfwGetTime() - freeCamToggleTime > freeCamToggleDelay) {
                setHasGravity(!hasGravity);
                freeCamToggleTime = glfwGetTime();
            }
            break;
        case FreeCamControls::FASTER_FREECAM:
            if (!hasGravity) {
                //Add speed to free camera
                freeCamSpeed += 0.01f;
            }
            break;
        case FreeCamControls::SLOWER_FREECAM:
            if (!hasGravity) {
                //Subtract speed from free camera make sure it's at least 0.05
                freeCamSpeed -= 0.01f;
                freeCamSpeed = fmax(freeCamSpeed, 0.05f);
            }
            break;
        case FreeCamControls::RESET_SPEED_FREECAM:
            if (!hasGravity) {
                //Set speed to default
                freeCamSpeed = 0.05f;
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

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(fov), (float) width / (float) height, 0.1f, 100.0f);
}

void Camera::setWindowDimensions(int width, int height) {
    this->width = width;
    this->height = height;
}

std::pair<Interactable *, float>
Camera::rayCast(const std::vector<Interactable *> &interactables, const std::vector<AxisAlignedBB> &colliders) const {
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
    auto [closestModel, distance] = rayCast(interactables, colliders);

    if (closestModel != nullptr && distance < MAX_INTERACT_DISTANCE) {
        closestModel->onInteract(position, distance);
    }
}

void Camera::doPhysics(float deltaTime, const std::vector<AxisAlignedBB> &colliders) {
    Entity::doPhysics(deltaTime, colliders);
    updateCameraVectors();

    if (!hasGravity) {
        velocity = glm::vec3(0.0f);
    }

    if (onGround) {
        if (wasOnGround) {
            if (didMove && (walkSound == nullptr || !walkSound->isPlaying())) {
                delete walkSound;
                walkSound = new Sound(Sound::getRandomSound("walk/gravel.ogg", 4), position, 0.2f);
                walkSound->play();
            }
        } else {
            Sound sound = Sound("fall.ogg", position, 0.5f);
            sound.play();
        }
    }

    didMove = false;
}

void Camera::setPosition(glm::vec3 position) {
    Entity::setPosition(position);
    updateCameraVectors();
}

void Camera::attack(const std::vector<Interactable *> &interactables, const std::vector<AxisAlignedBB> &colliders) {
    auto [closestModel, distance] = rayCast(interactables, colliders);

    if (closestModel != nullptr && distance < MAX_INTERACT_DISTANCE) {
        closestModel->onAttack(position, distance);
    }
}

void Camera::onAttack() {
    lastHitTime = glfwGetTime();

    Sound sound = Sound(Sound::getRandomSound("hit/hit.ogg", 3), position);
    sound.play();

    health--;
    if (health <= 0) {
        Sound sound = Sound("hit/death.ogg", position);
        sound.play();

        while (sound.isPlaying()) {
            // do nothing
        }

        exit(EXIT_SUCCESS);
    }
}

bool Camera::wasHit() const {
    return glfwGetTime() - lastHitTime < ATTACK_DELAY;
}
