#include "Obstacle.h"
#include "../../sound/Sound.h"

void Obstacle::onAttack(glm::vec3 position, float distance) {
    // do nothing
}

void Obstacle::onInteract(glm::vec3 position, float distance) {
    kill();

    auto pos = getPosition();
    Sound sound = Sound(Sound::getRandomSound("obstacle/dig.ogg", 4), pos);
    sound.play();

    pathfindingAlgorithm.removeObstacle({pos.x, pos.z});
}