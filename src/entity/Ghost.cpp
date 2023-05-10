#include "Ghost.h"
#include "../utils/ModelLoader.h"
#include "glm/ext/matrix_transform.hpp"
#include "GLFW/glfw3.h"
#include <random>

void Ghost::onAttack(glm::vec3 position, float distance) {
    health--;
    std::string soundPath;

    if (health <= 0) {
        soundPath = "ghost/death.ogg";
        kill();
    } else {
        soundPath = Sound::getRandomSound("hit/hit.ogg", 3);
        model->onHit();
        knockback(position, ATTACK_FORCE);
    }

    Sound sound = Sound(soundPath, getPosition());
    sound.play();
}

void Ghost::doPhysics(float deltaTime, const std::vector<AxisAlignedBB> &colliders) {
    if (!path.empty() && glfwGetTime() - lastSoundTime > SOUND_COOLDOWN) {
        lastSoundTime = glfwGetTime();
        Sound sound = Sound(Sound::getRandomSound("ghost/moan.ogg", 7), getPosition());
        sound.play();
    }

    AIEntity::doPhysics(deltaTime, colliders);
}

void Ghost::onInteract(glm::vec3 position, float distance) {
    // do nothing
}

Ghost::Ghost(glm::vec3 position) : AIEntity(position,
                                            AxisAlignedBB(glm::vec3(-0.3f, -0.8f, -0.3f), glm::vec3(0.3f, 0.8f, 0.3f)),
                                            0.0f, 0.0f), health(10) {
    ModelLoader *modelLoader = ModelLoader::getInstance();
    //generate random number between 1 and 6
    int random = rand() % 6;
    std::vector<Mesh *> meshes;
    switch (random) {
        case 0: {
            meshes = modelLoader->loadMeshes("objects/ghost/blue_ghost/blue_ghost.obj");
            break;
        }
        case 1: {
            meshes = modelLoader->loadMeshes("objects/ghost/orange_ghost/orange_ghost.obj");
            break;
        }
        case 2: {
            meshes = modelLoader->loadMeshes("objects/ghost/green_ghost/green_ghost.obj");
            break;
        }
        case 3: {
            meshes = modelLoader->loadMeshes("objects/ghost/pink_ghost/pink_ghost.obj");
            break;
        }
        case 4: {
            meshes = modelLoader->loadMeshes("objects/ghost/black_ghost/black_ghost.obj");
            break;
        }
        default:
            meshes = modelLoader->loadMeshes("objects/ghost/cyan_ghost/cyan_ghost.obj");
            break;
    }


    auto *aiModel = new Model(glm::rotate(glm::mat4(0.0f), glm::radians(180.0f), glm::vec3(0, 1, 0)),
                              new Shader("shaders/singular.vs", "shaders/lighting.fs"), meshes);
    setModel(aiModel);
}

bool Ghost::canAttack(glm::vec3 position) {
    return glm::distance(position, getPosition()) < ATTACK_DISTANCE && glfwGetTime() - lastAttackTime > ATTACK_DELAY;
}

void Ghost::attack() {
    lastAttackTime = glfwGetTime();
}
