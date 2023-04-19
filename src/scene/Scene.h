#ifndef OPENGL_PROJECT_SCENE_H
#define OPENGL_PROJECT_SCENE_H

#include "model/Model.h"
#include <vector>

class Scene{
public:
    Scene();
    ~Scene();
    void render(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix);
private:
    std::vector<Model* > objects;
    void addObject(Model* object);
    void renderMaze();

    static void bindToVector(std::vector<Vertex> &vertices, glm::vec3 vector, glm::vec2 texCoords);
};


#endif
