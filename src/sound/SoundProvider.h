#ifndef OPENGL_PROJECT_SOUNDPROVIDER_H
#define OPENGL_PROJECT_SOUNDPROVIDER_H
#ifdef __APPLE__
//#include <irrklang/irrKlang.h>
#endif

#include "../camera/Camera.h"

class SoundProvider {
private:
    irrklang::ISoundEngine *engine;
    static SoundProvider *instance;

    SoundProvider();

    ~SoundProvider();

public:
    static SoundProvider *getInstance();

    void updateCameraPosition(glm::vec3 position, glm::vec3 front, glm::vec3 up);

    irrklang::ISoundEngine *getEngine();
};


#endif
