#include "Obstacle.h"
#include "../../sound/Sound.h"

void Obstacle::onAttack(glm::vec3 position, float distance) {
    // do nothing
}

void Obstacle::onInteract(glm::vec3 position, float distance) {
    kill();

    Sound sound = Sound(Sound::getRandomSound("obstacle/dig.ogg", 4), getPosition());
    sound.play();
}