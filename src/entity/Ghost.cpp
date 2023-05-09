#include "Ghost.h"
#include "../utils/ModelLoader.h"
#include "glm/ext/matrix_transform.hpp"
#include "GLFW/glfw3.h"

void Ghost::onAttack(glm::vec3 position, float distance) {
    health--;
    std::string soundPath;

    if (health <= 0) {
        soundPath = "ghost/death.ogg";
        kill();
    } else {
        soundPath = Sound::getRandomSound("hit/hit.ogg", 3);
        model->onHit();

        glm::vec3 direction = glm::normalize(position - getPosition());
        knockback(direction, 0.5f);
    }

    Sound sound = Sound(soundPath, getPosition());
    sound.play();
}

void Ghost::onInteract(glm::vec3 position, float distance) {
    // do nothing
}

Ghost::Ghost(glm::vec3 position) : AIEntity(position,AxisAlignedBB(glm::vec3(-0.1f, -0.8f, -0.1f), glm::vec3(0.1f, 0.8f, 0.1f)), 0.0f,0.0f), health(10) {
    ModelLoader *modelLoader = ModelLoader::getInstance();
    std::vector<Mesh *> meshes = modelLoader->loadMeshes("objects/ghost/ghost.obj");

    auto *aiModel = new Model(glm::rotate(glm::mat4(0.0f), glm::radians(180.0f), glm::vec3(0, 1, 0)), new Shader("shaders/singular.vs", "shaders/lighting.fs"), meshes);
    setModel(aiModel);
}