#include "CustomMazeParser.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
//----CONSTANTS-------------------------------------------------------


#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3
//----GLOBAL VARIABLES------------------------------------------------
vector<vector<char>> maze;


//----FUNCTIONS-------------------------------------------------------
void CustomMazeParser::saveGridToFile(char* filename) {
    ofstream myfile;
    myfile.open(filename);


    // Displays the finished maze to the screen.
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            myfile << maze[x][y];
        }
        myfile << endl;
    }
    myfile.close();

}

void CustomMazeParser::createEntranceAndExit() {
    // Create an entrance and exit.
    maze[0][1] = ' ';
    maze[0][0] = ' ';
    maze[1][0] = ' ';
    maze[width - 1][height - 2] = ' ';
    maze[width - 2][height - 1] = ' ';
    maze[width - 1][height - 1] = ' ';

}


void CustomMazeParser::ResetGrid() {

    //make maze 2d vector of chars with height and width
    vector<char> row(width, '#');
    for (int i = 0; i < height; i++) {
        maze.push_back(row);
    }
}

int CustomMazeParser::IsInBounds(int x, int y) {
    // Returns "true" if x and y are both in-bounds.
    if (x < 0 || x >= width) return false;
    if (y < 0 || y >= height) return false;
    return true;
}

// This is the recursive function we will code in the next project
void CustomMazeParser::goToSpace(int x, int y) {
    // Starting at the given index, recursively visits every direction in a
    // randomized order.
    // Set my current location to be an empty passage.
    maze[x][y] = ' ';
    // Create an local array containing the 4 directions and shuffle their order.
    int dirs[4];
    dirs[0] = NORTH;
    dirs[1] = EAST;
    dirs[2] = SOUTH;
    dirs[3] = WEST;
    for (int i = 0; i < 4; ++i) {
        int r = rand() & 3;
        int temp = dirs[r];
        dirs[r] = dirs[i];
        dirs[i] = temp;
    }
    // Loop through every direction and attempt to goToSpace that direction.
    for (int i = 0; i < 4; ++i) {
        // dx,dy are offsets from current location. Set them based
        // on the next direction I wish to try.
        int dx = 0, dy = 0;
        switch (dirs[i]) {
            case NORTH:
                dy = -1;
                break;
            case SOUTH:
                dy = 1;
                break;
            case EAST:
                dx = 1;
                break;
            case WEST:
                dx = -1;
                break;
        }
        // Find the (x,y) coordinates of the grid cell 2 spots
        // away in the given direction.
        int x2 = x + (dx << 1);
        int y2 = y + (dy << 1);
        if (IsInBounds(x2, y2)) {
            if (maze[x2][y2] == '#') {
                // (x2,y2) has not been visited yet... knock down the
                // wall between my current position and that position
                maze[x2 - dx][y2 - dy] = ' ';
                // Recursively goToSpace (x2,y2)
                goToSpace(x2, y2);
            }
        }
    }
}

void CustomMazeParser::PrintGrid() {
    // Displays the finished maze to the screen.
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            cout << maze[x][y];
        }
        cout << endl;
    }
}
void CustomMazeParser::checkHeightAndWidth(int height, int width) {
    //make sure height and width are odd
    if (height % 2 == 0 || width % 2 == 0) {
        cout << "Height and width must be odd numbers" << endl;
        exit(1);
    }
    //make sure height and width are greater than 5
    if (height < 5 || width < 5) {
        cout << "Height and width must be greater than 5" << endl;
        exit(1);
    }
    //make sure height is at least the half of width
    if (height < width / 2) {
        cout << "Height must be at least half of width" << endl;
        exit(1);
    }
    //make sure height is not more than width
    if (height > width) {
        cout << "Height must be less than width" << endl;
        exit(1);
    }

}

CustomMazeParser::CustomMazeParser(int height, int width) {
    checkHeightAndWidth(height, width);
    this->height = height;
    this->width = width;
//make maze 2d vector of chars with height and width
    vector<char> row(width, '#');
    for (int i = 0; i < height; i++) {
        maze.push_back(row);
    }

    // Starting point and top-level control.

    srand(time(0)); // seed random number generator.
    ResetGrid();
    goToSpace(1, 1);
    createEntranceAndExit();
    saveGridToFile("../resources/maze/maze.txt");
    PrintGrid();

}