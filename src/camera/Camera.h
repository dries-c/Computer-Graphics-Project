#ifndef OPENGL_PROJECT_CAMERA_H
#define OPENGL_PROJECT_CAMERA_H

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "Input.h"
#include "FreeCamControls.h"
#include "../utils/AxisAlignedBB.h"
#include "../entity/Entity.h"
#include "../utils/Interactable.h"
#include "../sound/Sound.h"
#include <string>

#define SPEED 0.04f

class Camera : public Entity {
protected:
    static Camera *instance;

    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);

private:
    glm::vec3 front{};
    glm::vec3 up{};
    glm::vec3 right{};
    glm::vec3 worldUp{};

    Sound* walkSound = nullptr;

    float freeCamSpeed = 0.05f;
    bool didMove = false;
    double freeCamToggleTime = 0.0f;
    const double freeCamToggleDelay = 0.2f;

    int width{};
    int height{};

    float sensitivity;
    float fov;

    double lastHitTime = 0.0f;

    void updateCameraVectors();
    [[nodiscard]] std::pair<Interactable*, float> rayCast(const std::vector<Interactable *> &interactables, const std::vector<AxisAlignedBB> &colliders) const;
public:
    Camera(Camera &other) = delete;

    void operator=(const Camera &) = delete;

    static Camera *getInstance(glm::vec3 position = glm::vec3(0.5f, 0.8f, 0.5f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = 45.0f, float pitch = 0.0f);

    void doPhysics(float deltaTime, const std::vector<AxisAlignedBB> &colliders) override;

    void setPosition(glm::vec3 position) override;

    void processMouseMovement(float xoffset, float yoffset);

    void processMouseScroll(float yoffset);

    void processKeyboard(Input direction);

    void onAttack();

    bool wasHit() const;

    void processKeyboard(FreeCamControls direction);

    [[nodiscard]] std::string toString() const;

    [[nodiscard]] glm::mat4 getViewMatrix() const;

    [[nodiscard]] glm::mat4 getProjectionMatrix() const;

    void setWindowDimensions(int width, int height);

    void interact(const std::vector<Interactable *> &interactables, const std::vector<AxisAlignedBB> &colliders);

    void attack(const std::vector<Interactable *> &interactables, const std::vector<AxisAlignedBB> &colliders);
};

#endif
