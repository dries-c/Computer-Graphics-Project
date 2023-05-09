#include "Obstacle.h"
#include "../../sound/Sound.h"

void Obstacle::onAttack(float distance) {
    // do nothing
}

void Obstacle::onInteract(float distance) {
    kill();

    Sound sound = Sound(Sound::getRandomSound("obstacle/dig.ogg", 4), getPosition());
    sound.play();
}