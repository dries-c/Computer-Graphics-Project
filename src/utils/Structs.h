#ifndef OPENGL_PROJECT_STRUCTS_H
#define OPENGL_PROJECT_STRUCTS_H

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};


#endif
