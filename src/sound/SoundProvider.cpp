#include "SoundProvider.h"

SoundProvider *SoundProvider::instance = nullptr;

SoundProvider *SoundProvider::getInstance() {
    if (instance == nullptr) {
        instance = new SoundProvider();
    }

    return instance;
}


SoundProvider::SoundProvider() {
    engine = irrklang::createIrrKlangDevice();
}

SoundProvider::~SoundProvider() {
    engine->drop();
}

void SoundProvider::updateCameraPosition(glm::vec3 position, glm::vec3 front, glm::vec3 up) {
    engine->setListenerPosition(
            irrklang::vec3df(position.x, position.y, position.z),
            irrklang::vec3df(front.x, front.y, front.z),
            irrklang::vec3df(up.x, up.y, up.z)
    );
}

irrklang::ISoundEngine * SoundProvider::getEngine() {
    return engine;
}
