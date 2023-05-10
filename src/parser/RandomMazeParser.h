

#ifndef OPENGL_PROJECT_RANDOMMAZEPARSER_H
#define OPENGL_PROJECT_RANDOMMAZEPARSER_H

#include "MazeParser.h"


class RandomMazeParser : public MazeParser {
private:
    int height;
    int width;


    void resetMaze();
    int IsInBounds(int x, int y);
    void goToSpace(int x, int y);
    void PrintMaze();
    void saveMazeToFile(const char *filename);
    void createExit();
    void checkHeightAndWidth(int height, int width);
    void placeLights();
    void placeGhosts();


public:
    explicit RandomMazeParser(int height, int width, const char* fileName);

    void placeObstacles();
};


#endif //OPENGL_PROJECT_RANDOMMAZEPARSER_H
