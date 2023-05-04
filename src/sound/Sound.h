#ifndef OPENGL_PROJECT_SOUND_H
#define OPENGL_PROJECT_SOUND_H


#include <string>
#include "glm/vec3.hpp"
#ifdef __APPLE__
#include "irrKlang/ik_ISound.h"
#endif

class Sound {
private:
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    std::string filePath;
    float volume;
    bool loop;
    bool ambient;

#ifdef __APPLE__
    irrklang::ISound *instance;
#endif
public:
    Sound(std::string filePath, glm::vec3 position, float volume = 1.0f, bool loop = false);
    explicit Sound(std::string filePath, float volume = 1.0f, bool loop = false);
    bool isPlaying();
    void pause(bool pause);
    void setPosition(glm::vec3 position);
    void setVolume(float volume);
    void stop();
    void play();
};


#endif
