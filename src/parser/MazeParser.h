#ifndef OPENGL_PROJECT_MAZEPARSER_H
#define OPENGL_PROJECT_MAZEPARSER_H

#include <vector>
#include "PositionEnum.h"

class MazeParser {
protected:
    // [x][y] = Enum
    std::vector<std::vector<PositionEnum>> maze;

public:
    std::vector<std::vector<PositionEnum>> getMaze();
};


#endif
