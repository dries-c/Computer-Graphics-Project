#include "AIEntity.h"
#include "glm/ext/matrix_transform.hpp"

#define SPEED 2.0f

void AIEntity::doPhysics(float deltaTime, const std::vector<AxisAlignedBB> &colliders) {
    if (!path.empty()) {
        glm::vec2 top = path.top();
        glm::vec2 target = glm::vec2(top.x + 0.5f, top.y + 0.5f);
        glm::vec2 current = glm::vec2(position.x, position.z);

        if(glm::distance(target, current) < 0.1f) {
            path.pop();
        } else {
            glm::vec2 direction = glm::normalize(target - current);
            velocity.x = direction.x * SPEED;
            velocity.z = direction.y * SPEED;

            float correctYaw = glm::degrees(atan2(direction.x, direction.y));
            yaw += (correctYaw - yaw) * 0.1f;
        }
    }

    Entity::doPhysics(deltaTime, colliders);
}

void AIEntity::updatePath(PathFindingAlgorithm *pathFinding, glm::vec3 target) {
    glm::vec2 current = glm::vec2(int(position.x), int(position.z));
    glm::vec2 goal = glm::vec2(int(target.x), int(target.z));
    path = pathFinding->getPath(current, goal);
}
