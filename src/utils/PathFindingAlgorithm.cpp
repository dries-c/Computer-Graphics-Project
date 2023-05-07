#include "PathFindingAlgorithm.h"
#include "glm/geometric.hpp"

#include <stack>

PathFindingAlgorithm::PathFindingAlgorithm(std::vector<std::vector<bool>> walkableMaze) : walkableMaze(
        std::move(walkableMaze)) {}

std::stack<glm::vec2> PathFindingAlgorithm::getPath(const glm::vec2 &start, const glm::vec2 &target) {
    std::vector<Node *> openList;
    std::vector<Node *> closedList;

    Node *startNode = new Node{start, nullptr, 0};
    openList.push_back(startNode);

    while (!openList.empty()) {
        std::sort(openList.begin(), openList.end(), [](const Node *a, const Node *b) {
            return a->score > b->score;
        });

        Node *current = openList.back();
        openList.pop_back();

        if (current->position == target) {
            return reconstructPath(current);
        }

        closedList.push_back(current);
        for (auto neighbour: getNeighbours(current->position)) {
            if (!isPositionInList(closedList, neighbour)) {
                float score = current->score + heuristic(neighbour, target);

                int index = getIndex(openList, neighbour);
                if (index == -1) {
                    openList.push_back(new Node{neighbour, current, score});
                } else {
                    Node *neighbourNode = openList[index];

                    if (current->score < neighbourNode->score) {
                        neighbourNode->parent = current;
                        neighbourNode->score = score;
                    }
                }
            }
        }
    }

    return {};
}

std::vector<glm::vec2> PathFindingAlgorithm::getNeighbours(const glm::vec2 &current) const {
    std::vector<glm::vec2> walkableNeighbours;

    for (auto neighbour: {
            current + glm::vec2(1, 0),
            current + glm::vec2(-1, 0),
            current + glm::vec2(0, 1),
            current + glm::vec2(0, -1),
    }) {
        if (isWalkable(neighbour)) {
            walkableNeighbours.push_back(neighbour);
        }
    }

    return walkableNeighbours;
}

bool PathFindingAlgorithm::isWalkable(const glm::vec2 &position) const {
    if (position.x < 0 || position.y < 0) {
        return false;
    }

    std::vector<bool> row = walkableMaze.at(position.x);
    if (position.y >= row.size()) {
        return false;
    }

    return row[position.y];
}

float PathFindingAlgorithm::heuristic(const glm::vec2 &a, const glm::vec2 &b) {
    return glm::distance(a, b);
}

std::stack<glm::vec2> PathFindingAlgorithm::reconstructPath(Node *node) {
    std::stack<glm::vec2> path;

    while (node->parent) {
        path.push(node->position);
        node = node->parent;
    }

    return path;
}

bool PathFindingAlgorithm::isPositionInList(const std::vector<Node *> &list, const glm::vec2 &position) {
    return std::find_if(list.begin(), list.end(), [position](const Node *node) {
        return node->position == position;
    }) != list.end();
}

int PathFindingAlgorithm::getIndex(const std::vector<Node *> &list, const glm::vec2 &position) {
    auto it = std::find_if(list.begin(), list.end(), [position](const Node *node) {
        return node->position == position;
    });

    if (it == list.end()) {
        return -1;
    }

    return it - list.begin();
}
