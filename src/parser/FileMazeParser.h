#ifndef OPENGL_PROJECT_FILEMAZEPARSER_H
#define OPENGL_PROJECT_FILEMAZEPARSER_H


#include "MazeParser.h"

class FileMazeParser: public MazeParser {
public:
    explicit FileMazeParser(const char *path);
};


#endif
