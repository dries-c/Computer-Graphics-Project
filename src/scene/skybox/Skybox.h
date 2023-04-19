#ifndef OPENGL_PROJECT_SKYBOX_H
#define OPENGL_PROJECT_SKYBOX_H

#include "../model/shader/Shader.h"
#include "../../utils/Structs.h"
#include "../mesh/Mesh.h"
#include "SkyboxTexture.h"

class Skybox {
private:
    Shader *shader;
    Mesh *mesh;
    SkyboxTexture *texture;

    static void bindToVector(std::vector<Vertex> &vertices, glm::vec3 vector);
public:
    Skybox(std::vector<std::string> faces);
    void render(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix);
    ~Skybox();
};


#endif
