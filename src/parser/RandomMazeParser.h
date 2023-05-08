

#ifndef OPENGL_PROJECT_RANDOMMAZEPARSER_H
#define OPENGL_PROJECT_RANDOMMAZEPARSER_H

#include "MazeParser.h"


class RandomMazeParser : public MazeParser {
private:
    int height;
    int width;


    void ResetMaze();
    int XYToIndex(int x, int y);
    int IsInBounds(int x, int y);
    void goToSpace(int x, int y);
    void PrintMaze();
    void saveMazeToFile(const char *filename);
    void createEntranceAndExit();
    void checkHeightAndWidth(int height, int width);
    void placeLights();
    void fileMazeParser( const char* filename);


public:
    explicit RandomMazeParser(int height, int width, const char* fileName);

    void placeObstacles();
};


#endif //OPENGL_PROJECT_RANDOMMAZEPARSER_H
