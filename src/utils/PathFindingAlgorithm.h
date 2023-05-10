#ifndef OPENGL_PROJECT_PATHFINDINGALGORITHM_H
#define OPENGL_PROJECT_PATHFINDINGALGORITHM_H
#include <vector>
#include <stack>
#include "glm/vec2.hpp"

struct Node {
    glm::vec2 position;
    Node *parent;
    float score;

    bool operator<(const Node& other) const {
        return score > other.score;
    }
};

class PathFindingAlgorithm {
private:
    std::vector<std::vector<bool>> walkableMaze;

    [[nodiscard]] std::vector<glm::vec2> getNeighbours(const glm::vec2 &current) const;
    [[nodiscard]] bool isWalkable(const glm::vec2 &position) const;
    static std::stack<glm::vec2> reconstructPath(Node *current);
    static float heuristic(const glm::vec2& a, const glm::vec2& b);
    static bool isPositionInList(const std::vector<Node*>& list, const glm::vec2& position);
    static int getIndex(const std::vector<Node*>& list, const glm::vec2& position);

public:
    explicit PathFindingAlgorithm(std::vector<std::vector<bool>> walkableMaze);
    std::stack<glm::vec2> getPath(const glm::vec2& start, const glm::vec2& target);

    void removeObstacle(const glm::vec2& position);
};


#endif
