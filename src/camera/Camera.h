#ifndef OPENGL_PROJECT_CAMERA_H
#define OPENGL_PROJECT_CAMERA_H

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "Direction.h"
#include <string>

#define GRAVITY 9.81f
#define JUMP_SPEED 4.0f
#define MIN_Y 0.0f
#define SPEED 2.0f

class Camera{
protected:
    static Camera* instance;
    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);
private:
    glm::vec3 position{};
    glm::vec3 front{};
    glm::vec3 up{};
    glm::vec3 right{};
    glm::vec3 worldUp{};
    glm::vec3 velocity{};

    bool freeCamera = true;

    int width{};
    int height{};

    float yaw;
    float pitch;
    float sensitivity;
    float fov;
    void updateCameraVectors();
    bool isOnGround() const;

public:
    void physicsUpdate(float deltaTime);
    Camera(Camera &other) = delete;
    void operator=(const Camera &) = delete;

    static Camera* getInstance(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f);

    void processMouseMovement(float xoffset, float yoffset);
    void processMouseScroll(float yoffset);
    void processKeyboard(Direction direction);
    [[nodiscard]] std::string toString() const;
    [[nodiscard]] glm::mat4 getViewMatrix() const;
    [[nodiscard]] glm::mat4 getProjectionMatrix() const;

    void setWindowDimensions(int width, int height);
};

#endif
