#include "Sound.h"

#include <utility>
#include "../utils/FileUtils.h"
#include "SoundProvider.h"

void Sound::play() {
    std::string path = RESOURCES_PATH + "sounds/" + filePath;
    irrklang::ISoundEngine *engine = SoundProvider::getInstance()->getEngine();

    if (ambient) {
        instance = engine->play2D(path.c_str(), loop, false, true);
    } else {
        instance = engine->play3D(
                path.c_str(),
                irrklang::vec3df(position.x, position.y, position.z),
                loop,
                false,
                true
        );
    }

    instance->setVolume(volume);
}

void Sound::stop() {
    if (instance == nullptr) {
        throw std::runtime_error("Sound instance is null");
    }

    instance->stop();
}

void Sound::setVolume(float volume) {
    this->volume = volume;

    if (instance != nullptr) {
        instance->setVolume(volume);
    }
}

void Sound::setPosition(glm::vec3 position) {
    this->position = position;

    if (instance != nullptr) {
        instance->setPosition(irrklang::vec3df(position.x, position.y, position.z));
    }
}

void Sound::pause(bool pause = true) {
    if (instance == nullptr) {
        throw std::runtime_error("Sound instance is null");
    }

    instance->setIsPaused(pause);
}

bool Sound::isPlaying() {
    if (instance == nullptr) {
        throw std::runtime_error("Sound instance is null");
    }

    return !instance->isFinished() && !instance->getIsPaused();
}

Sound::Sound(std::string filePath, glm::vec3 position, float volume, bool loop) : filePath(std::move(filePath)),
                                                                                  position(position), volume(volume),
                                                                                  loop(loop), ambient(false) {}

Sound::Sound(std::string filePath, float volume, bool loop) : filePath(std::move(filePath)), volume(volume), loop(loop),
                                                              ambient(true) {}
