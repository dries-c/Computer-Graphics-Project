#ifndef OPENGL_PROJECT_CAMERA_H
#define OPENGL_PROJECT_CAMERA_H

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "Direction.h"
#include <string>

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

    int width{};
    int height{};

    float yaw;
    float pitch;
    float sensitivity;
    float fov;
    float speed;
    void updateCameraVectors();

public:
    Camera(Camera &other) = delete;
    void operator=(const Camera &) = delete;

    static Camera* getInstance(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = 90.0f, float pitch = 0.0f);

    void processMouseMovement(float xoffset, float yoffset);
    void processMouseScroll(float yoffset);
    void processKeyboard(Direction direction, float deltaTime);
    [[nodiscard]] std::string toString() const;
    [[nodiscard]] glm::mat4 getViewMatrix() const;
    [[nodiscard]] glm::mat4 getProjectionMatrix() const;

    void setWindowDimensions(int width, int height);
};

#endif
