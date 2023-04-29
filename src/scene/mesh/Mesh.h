#ifndef OPENGL_PROJECT_MESH_H
#define OPENGL_PROJECT_MESH_H

#include <vector>
#include "texture/Texture.h"
#include "../../utils/Structs.h"
#include "../../utils/AxisAlignedBB.h"
#include "../../entity/Entity.h"

class Mesh {
private:
    unsigned int VAO, VBO, EBO;
    std::vector<Texture *> textures;
    AxisAlignedBB boundingBox;

    unsigned int nextVertexAttribute = 0;
    int instanceCount = 0;
    unsigned int indicesCount;

public:
    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, const std::vector<Texture *> &textures);
    ~Mesh();

    /**
     * Render the mesh using the given shader
     *
     * @pre the shader must be bound before calling this method
     * @param shader the shader to use
     */
    void render(Shader &shader);

    /**
     * Setup the mesh for instancing
     *
     * @param count the number of instances
     */
    void setupInstancing(int count);

    /**
     * Get the bounding box of the mesh
     */
    AxisAlignedBB getBoundingBox();
};


#endif
