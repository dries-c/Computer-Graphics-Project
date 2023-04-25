#include "AxisAlignedBB.h"

bool AxisAlignedBB::intersects(AxisAlignedBB &bb) const {
    return minX < bb.maxX && maxX > bb.minX &&
           minY < bb.maxY && maxY > bb.minY &&
           minZ < bb.maxZ && maxZ > bb.minZ;
}

AxisAlignedBB::AxisAlignedBB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) : minX(minX),
                                                                                                       minY(minY),
                                                                                                       minZ(minZ),
                                                                                                       maxX(maxX),
                                                                                                       maxY(maxY),
                                                                                                       maxZ(maxZ) {}

AxisAlignedBB AxisAlignedBB::transform(glm::mat4 transformation) const {
    glm::vec4 min = transformation * glm::vec4(minX, minY, minZ, 1.0f);
    glm::vec4 max = transformation * glm::vec4(maxX, maxY, maxZ, 1.0f);

    return {min.x, min.y, min.z, max.x, max.y, max.z};
}

AxisAlignedBB::AxisAlignedBB(const std::vector<Vertex>& vertices) {
    minX = minY = minZ = std::numeric_limits<float>::max();
    maxX = maxY = maxZ = std::numeric_limits<float>::min();

    for (Vertex vertex : vertices) {
        if (vertex.position.x < minX) {
            minX = vertex.position.x;
        }
        if (vertex.position.y < minY) {
            minY = vertex.position.y;
        }
        if (vertex.position.z < minZ) {
            minZ = vertex.position.z;
        }
        if (vertex.position.x > maxX) {
            maxX = vertex.position.x;
        }
        if (vertex.position.y > maxY) {
            maxY = vertex.position.y;
        }
        if (vertex.position.z > maxZ) {
            maxZ = vertex.position.z;
        }
    }
}

AxisAlignedBB AxisAlignedBB::offset(glm::vec3 offset) const {
    return {minX + offset.x, minY + offset.y, minZ + offset.z, maxX + offset.x, maxY + offset.y, maxZ + offset.z};
}

glm::vec3 AxisAlignedBB::getCenter() const {
    return {(minX + maxX) / 2, (minY + maxY) / 2, (minZ + maxZ) / 2};
}
