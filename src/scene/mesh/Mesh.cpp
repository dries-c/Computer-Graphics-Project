#include "Mesh.h"

#include "glad/glad.h"
#include "texture/Texture.h"
#include "../../entity/Entity.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices,
           const std::vector<Texture *> &textures) : textures(textures), boundingBox(vertices) {
    indicesCount = indices.size();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // Positions attribute
    glEnableVertexAttribArray(nextVertexAttribute);
    glVertexAttribPointer(nextVertexAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) nullptr);
    nextVertexAttribute++;

    // Normals attribute
    glEnableVertexAttribArray(nextVertexAttribute);
    glVertexAttribPointer(nextVertexAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *) offsetof(Vertex, normal));
    nextVertexAttribute++;

    // Vertex texture coords
    glEnableVertexAttribArray(nextVertexAttribute);
    glVertexAttribPointer(nextVertexAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *) offsetof(Vertex, texCoords));
    nextVertexAttribute++;

    glBindVertexArray(0);
}

void Mesh::render(Shader &shader) {
    for (int i = 0; i < textures.size(); i++) {
        textures[i]->bind(shader, i);
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    if (instanceCount == 0) {
        glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);
    } else {
        glDrawElementsInstanced(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr, instanceCount);
    }
    glBindVertexArray(0);

    // unbind textures
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupInstancing(int count) {
    instanceCount = count;

    glBindVertexArray(VAO);

    // set the vertex attributes
    for (int i = 0; i < 4; i++) {
        glEnableVertexAttribArray(nextVertexAttribute);
        glVertexAttribPointer(nextVertexAttribute, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4),(void *) (i * sizeof(glm::vec4)));
        glVertexAttribDivisor(nextVertexAttribute, 1);
        nextVertexAttribute++;
    }

    glBindVertexArray(0);
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

AxisAlignedBB Mesh::getBoundingBox() {
    return boundingBox;
}
