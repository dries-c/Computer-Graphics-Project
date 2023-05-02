#ifndef OPENGL_PROJECT_AXISALIGNEDBB_H
#define OPENGL_PROJECT_AXISALIGNEDBB_H

#include "glm/ext/matrix_float4x4.hpp"
#include <vector>
#include "Structs.h"
#include "Face.h"

class AxisAlignedBB {
public:
    glm::vec3 min;
    glm::vec3 max;

    AxisAlignedBB(glm::vec3 min, glm::vec3 max);
    AxisAlignedBB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
    explicit AxisAlignedBB(const std::vector<Vertex> &vertices);
    [[nodiscard]] AxisAlignedBB offset(glm::vec3 offset) const;
    [[nodiscard]] AxisAlignedBB transform(glm::mat4 transformation) const;
    bool intersects(AxisAlignedBB &bb) const;

    [[nodiscard]] float getIntersectionDistance(const glm::vec3 &origin, const glm::vec3 &direction) const;
    [[nodiscard]] Face getIntersection(const AxisAlignedBB &bb) const;
    [[nodiscard]] glm::vec3 getCenter() const;
    [[nodiscard]] glm::vec3 getSize() const;

private:
    static float foldByComponent(glm::vec3 v, float (*f)(float, float));
};


#endif
