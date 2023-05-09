#include "MazeParser.h"

std::vector<std::vector<PositionEnum>> MazeParser::getMaze() {
    return maze;
}

std::vector<std::vector<bool>> MazeParser::getWalkableMaze() {
    std::vector<std::vector<bool>> walkableMaze;
    walkableMaze.reserve(maze.size());
    for (auto &i: maze) {
        std::vector<bool> row;
        row.reserve(i.size());
        for (auto position: i) {
            row.push_back(position == PositionEnum::EMPTY || position == PositionEnum::LIGHT || position == PositionEnum::GHOST);
        }
        walkableMaze.push_back(row);
    }
    return walkableMaze;
}
