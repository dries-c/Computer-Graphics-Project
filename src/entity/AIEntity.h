#ifndef OPENGL_PROJECT_AIENTITY_H
#define OPENGL_PROJECT_AIENTITY_H


#include "../utils/PathFindingAlgorithm.h"
#include "Entity.h"

class AIEntity : public Entity {
protected:
    std::stack<glm::vec2> path;
public:
    AIEntity(glm::vec3 position, AxisAlignedBB boundingBox, float yaw, float pitch) : Entity(position, boundingBox, yaw, pitch) {};
    void doPhysics(float deltaTime, const std::vector<AxisAlignedBB> &colliders) override;
    void updatePath(PathFindingAlgorithm *pathFinding, glm::vec3 target);
};


#endif
