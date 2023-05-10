#include "RandomMazeParser.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "../utils/FileUtils.h"
#include "PositionEnum.h"

using namespace std;
//----CONSTANTS-------------------------------------------------------
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

//----FUNCTIONS-------------------------------------------------------
void RandomMazeParser::saveMazeToFile(const char *filename) {
    ofstream myfile;
    //open the file with path "../resources/filename"
    myfile.open("../resources/" + string(filename));

    //check if the file is open
    if (!myfile.is_open()) {
        cout << "Error opening file";
        exit(1);
    }

    // Displays the finished maze to the screen.
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            switch (maze[x][y]) {
                case PositionEnum::WALL:
                    myfile << "#";
                    break;
                case PositionEnum::OBSTACLE:
                    myfile << "O";
                    break;
                case PositionEnum::EMPTY:
                    myfile << " ";
                    break;
                case PositionEnum::LIGHT:
                    myfile << "L";
                    break;
                case PositionEnum::GHOST:
                    myfile << "G";
            }
        }
        myfile << endl;
    }
    myfile.close();

}

void RandomMazeParser::createEntranceAndExit() {
    // Create an entrance and exit.
    //maze[0][1] = PositionEnum::EMPTY;
    //maze[0][0] = PositionEnum::EMPTY;
    //maze[1][0] = PositionEnum::EMPTY;
    maze[width - 1][height - 2] = PositionEnum::EMPTY;
    maze[width - 2][height - 1] = PositionEnum::EMPTY;
    maze[width - 1][height - 1] = PositionEnum::EMPTY;

}

void RandomMazeParser::placeLights() {

    maze[1][1] = PositionEnum::LIGHT;
    //place lights 'L' in all T shaped crossroads
    for (int i = 1; i < width - 1; i++) {
        for (int j = 1; j < height - 1; j++) {
            if (maze[i][j] == PositionEnum::EMPTY) {
                int count = 0;
                if (maze[i + 1][j] == PositionEnum::EMPTY) count++;
                if (maze[i - 1][j] == PositionEnum::EMPTY) count++;
                if (maze[i][j + 1] == PositionEnum::EMPTY) count++;
                if (maze[i][j - 1] == PositionEnum::EMPTY) count++;
                if (count == 3) {
                    maze[i][j] = PositionEnum::LIGHT;
                }
            }
        }
    }
}

void RandomMazeParser::placeObstacles() {
    //place doors 'O' on the place where empty spaces are surrounded by 2 empty spaces and there is no dead end behind the door
    //the count of doors is based on the width and height of the maze
    //the ration is 1 door per 36 spaces
    int count = 0;
    int max = (width * height) / 36;
    while (count < max) {
        int x = rand() % width;
        int y = rand() % height;
        if (maze[x][y] == PositionEnum::EMPTY) {
            int count2 = 0;
            if (maze[x - 1][y] == PositionEnum::EMPTY) count2++;
            if (maze[x + 1][y] == PositionEnum::EMPTY) count2++;
            if (maze[x][y + 1] == PositionEnum::EMPTY) count2++;
            if (maze[x][y - 1] == PositionEnum::EMPTY) count2++;
            if (count2 == 2) {
                if (maze[x + 1][y] == PositionEnum::EMPTY && maze[x - 1][y] == PositionEnum::EMPTY) {
                    if (maze[x][y + 1] == PositionEnum::WALL && maze[x][y - 1] == PositionEnum::WALL) {
                        maze[x][y] = PositionEnum::OBSTACLE;
                        count++;
                    }
                }
                if (maze[x][y + 1] == PositionEnum::EMPTY && maze[x][y - 1] == PositionEnum::EMPTY) {
                    if (maze[x + 1][y] == PositionEnum::WALL && maze[x - 1][y] == PositionEnum::WALL) {
                        maze[x][y] = PositionEnum::OBSTACLE;
                        count++;
                    }
                }
            }
        }
    }

}


void RandomMazeParser::ResetMaze() {


    //make maze 2d vector of chars with height and width
    vector<PositionEnum> row(width, PositionEnum::WALL);
    for (int i = 0; i < height; i++) {
        maze.push_back(row);
    }
}

int RandomMazeParser::IsInBounds(int x, int y) {
    // Returns "true" if x and y are both in-bounds.
    if (x < 0 || x >= width) return false;
    if (y < 0 || y >= height) return false;
    return true;
}

// This is the recursive function we will code in the next project
void RandomMazeParser::goToSpace(int x, int y) {
    // Starting at the given index, recursively visits every direction in a
    // randomized order.
    // Set my current location to be an empty passage.
    maze[x][y] = PositionEnum::EMPTY;;
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
            if (maze[x2][y2] == PositionEnum::WALL) {
                // (x2,y2) has not been visited yet... knock down the
                // wall between my current position and that position
                maze[x2 - dx][y2 - dy] = PositionEnum::EMPTY;
                // Recursively goToSpace (x2,y2)
                goToSpace(x2, y2);
            }
        }
    }
}

void RandomMazeParser::PrintMaze() {
    // Displays the finished maze to the screen.
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            switch (maze[x][y]) {
                case PositionEnum::WALL:
                    cout << "#";
                    break;
                case PositionEnum::EMPTY:
                    cout << " ";
                    break;
                case PositionEnum::OBSTACLE:
                    cout << "O";
                    break;
                case PositionEnum::LIGHT:
                    cout << "L";
                    break;

            }
        }
        cout << endl;
    }
}

void RandomMazeParser::checkHeightAndWidth(int height, int width) {
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


RandomMazeParser::RandomMazeParser(int height, int width, const char *fileName) {

    checkHeightAndWidth(height, width);
    this->height = height;
    this->width = width;

    vector<PositionEnum> row(width, PositionEnum::WALL);
    for (int i = 0; i < height; i++) {
        maze.push_back(row);
    }

    // Starting point and top-level control.

    srand(time(0)); // seed random number generator.
    //ResetMaze();
    goToSpace(1, 1);
    placeObstacles();
    createEntranceAndExit();
    placeLights();
    saveMazeToFile(fileName);
    PrintMaze();
}