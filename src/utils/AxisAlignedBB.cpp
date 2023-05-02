#include "AxisAlignedBB.h"
#include <iostream>

bool AxisAlignedBB::intersects(AxisAlignedBB &bb) const {
    return (min.x <= bb.max.x && max.x >= bb.min.x) &&
           (min.y <= bb.max.y && max.y >= bb.min.y) &&
           (min.z <= bb.max.z && max.z >= bb.min.z);
}

AxisAlignedBB::AxisAlignedBB(glm::vec3 min, glm::vec3 max) : min(min), max(max) {}

AxisAlignedBB::AxisAlignedBB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) : min(minX, minY,
                                                                                                           minZ),
                                                                                                       max(maxX, maxY,
                                                                                                           maxZ) {}

AxisAlignedBB AxisAlignedBB::transform(glm::mat4 transformation) const {
    return {transformation * glm::vec4(min, 1.0f), transformation * glm::vec4(max, 1.0f)};
}

AxisAlignedBB::AxisAlignedBB(const std::vector<Vertex> &vertices) {
    float maxVal = std::numeric_limits<float>::max();
    float minVal = std::numeric_limits<float>::min();

    min = {maxVal, maxVal, maxVal};
    max = {minVal, minVal, minVal};

    for (auto vertex: vertices) {
        glm::vec3 position = vertex.position;

        if (position.x < min.x) min.x = position.x;
        if (position.y < min.y) min.y = position.y;
        if (position.z < min.z) min.z = position.z;

        if (position.x > max.x) max.x = position.x;
        if (position.y > max.y) max.y = position.y;
        if (position.z > max.z) max.z = position.z;
    }
}

AxisAlignedBB AxisAlignedBB::offset(glm::vec3 offset) const {
    return {min + offset, max + offset};
}

float AxisAlignedBB::getIntersectionDistance(const glm::vec3 &origin, const glm::vec3 &direction) const {
    glm::vec3 minTime = (min - origin) / direction; // travel time to reach min
    glm::vec3 maxTime = (max - origin) / direction; // travel time to reach max

    float tMin = foldByComponent(glm::min(minTime, maxTime), glm::max); // max of mins
    float tMax = foldByComponent(glm::max(minTime, maxTime), glm::min); // min of maxs

    if (tMax >= tMin) {
        return foldByComponent(glm::min(minTime, maxTime), glm::max);
    }

    return -1;
}

float AxisAlignedBB::foldByComponent(glm::vec3 v, float (*f)(float, float)) {
    return f(v.x, f(v.y, v.z));
}

Face AxisAlignedBB::getIntersection(const AxisAlignedBB &bb) const {
    glm::vec3 center = getCenter();
    glm::vec3 bbCenter = bb.getCenter();
    glm::vec3 delta = center - bbCenter;
    glm::vec3 absDelta = glm::abs(delta);

    float maxComponent = foldByComponent(absDelta, glm::max);

    if (maxComponent == absDelta.x) {
        return delta.x > 0 ? Face::FACE_LEFT : Face::FACE_RIGHT;
    } else if (maxComponent == absDelta.z) {
        return delta.z > 0 ? Face::FACE_BACK : Face::FACE_FRONT;
    } else if (maxComponent == absDelta.y) {
        return delta.y > 0 ? Face::FACE_TOP : Face::FACE_BOTTOM;
    }

    throw std::runtime_error("No intersection found");
}

glm::vec3 AxisAlignedBB::getCenter() const {
    return (min + max) / 2.0f;
}

glm::vec3 AxisAlignedBB::getSize() const {
    return max - min;
}
