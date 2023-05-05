

#ifndef OPENGL_PROJECT_CUSTOMMAZEPARSER_H
#define OPENGL_PROJECT_CUSTOMMAZEPARSER_H


class CustomMazeParser {
private:
    int height;
    int width;

    void ResetGrid();
    int XYToIndex(int x, int y);
    int IsInBounds(int x, int y);
    void goToSpace(int x, int y);
    void PrintGrid();
    void saveGridToFile(char *filename);
    void createEntranceAndExit();
    void checkHeightAndWidth(int height, int width);
    void placeLights();


public:
    CustomMazeParser(int height, int width);

};


#endif //OPENGL_PROJECT_CUSTOMMAZEPARSER_H
