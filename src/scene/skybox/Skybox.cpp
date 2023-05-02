#include "Skybox.h"
#include "glad/glad.h"

Skybox::Skybox(std::vector<std::string> faces) : shader(new Shader("shaders/skybox.vs", "shaders/skybox.fs")),
                                                 texture(new SkyboxTexture(faces)) {
    std::vector<Vertex> vertices;
    bindToVector(vertices, glm::vec3(-1.0f, -1.0f, 1.0f));
    bindToVector(vertices, glm::vec3(1.0f, -1.0f, 1.0f));
    bindToVector(vertices, glm::vec3(1.0f, 1.0f, 1.0f));
    bindToVector(vertices, glm::vec3(-1.0f, 1.0f, 1.0f));

    bindToVector(vertices, glm::vec3(-1.0f, -1.0f, -1.0f));
    bindToVector(vertices, glm::vec3(1.0f, -1.0f, -1.0f));
    bindToVector(vertices, glm::vec3(1.0f, 1.0f, -1.0f));
    bindToVector(vertices, glm::vec3(-1.0f, 1.0f, -1.0f));

    std::vector<unsigned int> indices = {
            0, 1, 2,
            2, 3, 0,

            1, 5, 6,
            6, 2, 1,

            7, 6, 5,
            5, 4, 7,

            4, 0, 3,
            3, 7, 4,

            4, 5, 1,
            1, 0, 4,

            3, 2, 6,
            6, 7, 3
    };

    mesh = new Mesh(vertices, indices, {});
}

void Skybox::bindToVector(std::vector<Vertex> &vertices, glm::vec3 vector) {
    Vertex vertex = Vertex();
    vertex.position = vector;
    vertices.push_back(vertex);
}

void Skybox::render(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix) {
    glDepthFunc(GL_LEQUAL);
    shader->bind();
    shader->setMat4("view", glm::mat4(glm::mat3(viewMatrix))); // remove translation from the view matrix
    shader->setMat4("projection", projectionMatrix);
    texture->bind(*shader);
    mesh->render(*shader);
    glDepthFunc(GL_LESS);
}

Skybox::~Skybox() {
    delete shader;
    delete mesh;
    delete texture;
}
