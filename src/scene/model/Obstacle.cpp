#include "Obstacle.h"
#include "../../sound/Sound.h"

void Obstacle::onAttack() {
    // do nothing
}

void Obstacle::onInteract() {
    kill();
    Sound sound = Sound("break.ogg");
    sound.play();
}