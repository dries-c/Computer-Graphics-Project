#include <iostream>
#include "src/camera/Camera.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "src/scene/Scene.h"
#include "src/parser/FileMazeParser.h"

void setupGlfw();
void bindGlad();
void processInput(Camera *camera, GLFWwindow *window);
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
#ifdef __APPLE__
        std::cout << "FPS: " << 1.0 / deltaTime << std::endl;
#endif
        lastTime = currentTime;

        auto camera = Camera::getInstance();
        processInput(camera, window);
        camera->update(deltaTime, scene.getBoundingBoxes());

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 viewMatrix = camera->getViewMatrix();
        glm::mat4 projectionMatrix = camera->getProjectionMatrix();

        scene.render(viewMatrix, projectionMatrix);

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
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
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
        exit(EXIT_FAILURE);
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
    glEnable(GL_DEPTH_TEST);

    return window;
}

void processInput(Camera *camera, GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera->processKeyboard(INPUT_FORWARD);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera->processKeyboard(INPUT_BACKWARD);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera->processKeyboard(INPUT_LEFT);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera->processKeyboard(INPUT_RIGHT);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE)== GLFW_PRESS){
        camera->processKeyboard(INPUT_JUMP);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        camera->processKeyboard(INPUT_DOWN);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS){
        camera->processKeyboard(TOGGLE_FREECAM);
    }
    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS){
        camera->processKeyboard(FASTER_FREECAM);
    }
    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS){
        camera->processKeyboard(SLOWER_FREECAM);
    }
    if (glfwGetKey(window, GLFW_KEY_KP_0) == GLFW_PRESS){
        camera->processKeyboard(RESET_SPEED_FREECAM);
    }
}