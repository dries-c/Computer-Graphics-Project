#include "FileMazeParser.h"
#include "../utils/FileUtils.h"
#include "PositionEnum.h"
#include <iostream>

FileMazeParser::FileMazeParser(const char *path) {
    std::string mazeBuffer = FileUtils::getFileContents(path);

    std::vector<PositionEnum> row;
    for (char c: mazeBuffer) {
        switch (c) {
            case '#':
                row.push_back(PositionEnum::WALL);
                break;
            case 'O':
                row.push_back(PositionEnum::OBSTACLE);
                break;
            case ' ':
                row.push_back(PositionEnum::EMPTY);
                break;
            case 'L':
                row.push_back(PositionEnum::LIGHT);
                break;
            case 'G':
                row.push_back(PositionEnum::GHOST);
            case '\n':
                maze.push_back(row);
                row.clear();
                break;
            default:
                std::cout << "Unknown character: " << c << std::endl;
        }
    }

    maze.push_back(row);
    std::cout << "Maze loaded" << std::endl;
}