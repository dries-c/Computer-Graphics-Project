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
//----GLOBAL VARIABLES------------------------------------------------
vector<vector<char>> char_maze;


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
            myfile << char_maze[x][y];
        }
        myfile << endl;
    }
    myfile.close();

}

void RandomMazeParser::createEntranceAndExit() {
    // Create an entrance and exit.
    char_maze[0][1] = ' ';
    char_maze[0][0] = ' ';
    char_maze[1][0] = ' ';
    char_maze[width - 1][height - 2] = ' ';
    char_maze[width - 2][height - 1] = ' ';
    char_maze[width - 1][height - 1] = ' ';

}

void RandomMazeParser::placeLights() {
    //place lights 'L' on the place where walls are surrounded by 3 empty spaces
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            if (char_maze[x][y] == '#') {
                int count = 0;
                if (char_maze[x + 1][y] == '#') count++;
                if (char_maze[x - 1][y] == '#') count++;
                if (char_maze[x][y + 1] == '#') count++;
                if (char_maze[x][y - 1] == '#') count++;
                if (count >= 3) {
                    char_maze[x][y] = 'L';
                }
            }
        }
    }
}

void RandomMazeParser::placeDoors() {
    //place doors 'O' on the place where empty spaces are surrounded by 2 empty spaces and there is no dead end behind the door
    //the count of doors is based on the width and height of the maze
    //the ration is 1 door per 36 spaces
    int count = 0;
    int max = (width * height) / 36;
    while (count < max) {
        int x = rand() % width;
        int y = rand() % height;
        if (char_maze[x][y] == ' ') {
            int count2 = 0;
            if (char_maze[x + 1][y] == ' ') count2++;
            if (char_maze[x - 1][y] == ' ') count2++;
            if (char_maze[x][y + 1] == ' ') count2++;
            if (char_maze[x][y - 1] == ' ') count2++;
            if (count2 == 2) {
                if (char_maze[x + 1][y] == ' ' && char_maze[x - 1][y] == ' ') {
                    if (char_maze[x][y + 1] == '#' && char_maze[x][y - 1] == '#') {
                        char_maze[x][y] = 'O';
                        count++;
                    }
                }
                if (char_maze[x][y + 1] == ' ' && char_maze[x][y - 1] == ' ') {
                    if (char_maze[x + 1][y] == '#' && char_maze[x - 1][y] == '#') {
                        char_maze[x][y] = 'O';
                        count++;
                    }
                }
            }
        }
    }

}


void RandomMazeParser::ResetMaze() {

    //make maze 2d vector of chars with height and width
    vector<char> row(width, '#');
    for (int i = 0; i < height; i++) {
        char_maze.push_back(row);
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
    char_maze[x][y] = ' ';
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
            if (char_maze[x2][y2] == '#') {
                // (x2,y2) has not been visited yet... knock down the
                // wall between my current position and that position
                char_maze[x2 - dx][y2 - dy] = ' ';
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
            cout << char_maze[x][y];
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

void RandomMazeParser::fileMazeParser(const char *filename) {

    std::string mazeBuffer = FileUtils::getFileContents(filename);

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
                row.push_back(PositionEnum::WALL_WITH_LIGHT);
                break;
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

RandomMazeParser::RandomMazeParser(int height, int width, const char *fileName) {

    checkHeightAndWidth(height, width);
    this->height = height;
    this->width = width;
//make maze 2d vector of chars with height and width
    vector<char> row(width, '#');
    for (int i = 0; i < height; i++) {
        char_maze.push_back(row);
    }

    // Starting point and top-level control.

    srand(time(0)); // seed random number generator.
    ResetMaze();
    goToSpace(1, 1);
    placeDoors();
    createEntranceAndExit();
    placeLights();
    saveMazeToFile(fileName);
    PrintMaze();

    fileMazeParser(fileName);

}