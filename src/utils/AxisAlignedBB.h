#ifndef OPENGL_PROJECT_AXISALIGNEDBB_H
#define OPENGL_PROJECT_AXISALIGNEDBB_H

#include "glm/ext/matrix_float4x4.hpp"
#include <vector>
#include "Structs.h"

class AxisAlignedBB {
public:
    float minX;
    float minY;
    float minZ;
    float maxX;
    float maxY;
    float maxZ;

    AxisAlignedBB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
    explicit AxisAlignedBB(const std::vector<Vertex>& vertices);
    [[nodiscard]] AxisAlignedBB offset(glm::vec3 offset) const;
    [[nodiscard]] AxisAlignedBB transform(glm::mat4 transformation) const;
    bool intersects(AxisAlignedBB &bb) const;
    glm::vec3 getCenter() const;
};


#endif
