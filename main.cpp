#include <iostream>
#include "src/camera/Camera.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "src/scene/Scene.h"

void setupGlfw();

void bindGlad();

void processInput(Camera *camera, GLFWwindow *window, float deltaTime);

GLFWwindow *createWindow(int width, int height, const char *title);

int main() {
    setupGlfw();

    auto window = createWindow(800, 600, "OpenGL Project");

    double lastTime = glfwGetTime();
    float deltaTime;
    Scene scene = Scene();

    while (true) {
        if (glfwWindowShouldClose(window)) {
            glfwTerminate();
            return 0;
        }

        double currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        std::cout << "FPS: " << 1.0 / (currentTime - lastTime) << std::endl;
        lastTime = currentTime;

        auto camera = Camera::getInstance();

        processInput(camera, window, deltaTime);

        glm::mat4 viewMatrix = camera->getViewMatrix();
        glm::mat4 projectionMatrix = camera->getProjectionMatrix();

        scene.render(viewMatrix, projectionMatrix);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void setupGlfw() {
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        exit(-1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    glfwSetErrorCallback([](int error, const char *description) {
        fprintf(stderr, "Error: %s\n", description);
    });
}

void bindGlad() {
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }
}

GLFWwindow *createWindow(int width, int height, const char *title) {
    GLFWwindow *window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    Camera::getInstance()->setWindowDimensions(width, height);

    glfwMakeContextCurrent(window);

    bindGlad();

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
        Camera::getInstance()->setWindowDimensions(width, height);
    });
    glfwSetCursorPosCallback(window, [](GLFWwindow *window, double xpos, double ypos) {
        static double lastX = xpos;
        static double lastY = ypos;
        static bool firstMouse = true;

        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        double xoffset = xpos - lastX;
        double yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        Camera::getInstance()->processMouseMovement((float) xoffset, (float) yoffset);
    });
    glfwSetScrollCallback(window, [](GLFWwindow *window, double xoffset, double yoffset) {
        Camera::getInstance()->processMouseScroll((float) yoffset);
    });

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    return window;
}

void processInput(Camera *camera, GLFWwindow *window, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera->processKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera->processKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera->processKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera->processKeyboard(RIGHT, deltaTime);
    }
}