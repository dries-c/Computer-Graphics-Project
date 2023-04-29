#ifndef OPENGL_PROJECT_CAMERA_H
#define OPENGL_PROJECT_CAMERA_H

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "Input.h"
#include "FreeCamControls.h"
#include "../utils/AxisAlignedBB.h"
#include "../entity/Entity.h"
#include <string>

#define SPEED 3.0f

class Camera : public Entity {
protected:
    static Camera* instance;
    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);
private:
    glm::vec3 front{};
    glm::vec3 up{};
    glm::vec3 right{};
    glm::vec3 worldUp{};

    float FREE_CAM_SPEED = 2.0f;

    int width{};
    int height{};

    float sensitivity;
    float fov;
    void updateCameraVectors();

public:
    Camera(Camera &other) = delete;
    void operator=(const Camera &) = delete;

    static Camera* getInstance(glm::vec3 position = glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -45.0f, float pitch = 0.0f);

    void update(float deltaTime, const std::vector<AxisAlignedBB> &colliders) override;
    void setPosition(glm::vec3 position) override;

    void processMouseMovement(float xoffset, float yoffset);
    void processMouseScroll(float yoffset);
    void processKeyboard(Input direction);
    void processKeyboard(FreeCamControls direction);
    [[nodiscard]] std::string toString() const;
    [[nodiscard]] glm::mat4 getViewMatrix() const;
    [[nodiscard]] glm::mat4 getProjectionMatrix() const;

    void setWindowDimensions(int width, int height);
};

#endif
