#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

void Day6part1()
{
    enum direction { UP, RIGHT, DOWN, LEFT };
    direction dir = UP;
    int row = 0;
    int col = 0;

    std::fstream file("input6.txt");
    const int rows = 130;
    const int cols = 130;
    char grid[cols][rows];
    for (int i = 0; i < cols; i++) {
        std::string input;
        std::getline(file, input);
        for (size_t k = 0; k < input.length(); k++) {
            grid[i][k] = input[k];
            if (input[k] == '^') {
                col = i;
                row = k;
            }
            //std::cout << input[k];
        }
        //std::cout << std::endl;
    }
    //std::cout << std::endl;
    std::cout << "Start at col: " << col << ", row: " << row << std::endl;

    char visits[cols][rows];
    memset(visits, 0, sizeof(visits));

    bool outside = false;
    while (!outside) {
        // Map visited tiles
        visits[col][row]++;
        // Turn at obstructions or move
        switch (dir) {
            case UP:
                if (col > 0 && grid[col-1][row] == '#') {
                    dir = RIGHT;
                    //std::cout << "Direction right" << std::endl;
                } else {
                    col--;
                    //std::cout << "Up" << std::endl;
                }
                break;
            case RIGHT:
                if (row < (rows - 1) && grid[col][row+1] == '#') {
                    dir = DOWN;
                    //std::cout << "Direction down" << std::endl;
                } else {
                    row++;
                    //std::cout << "Right" << std::endl;
                }
                break;
            case DOWN:
                if (col < (cols - 1) && grid[col+1][row] == '#') {
                    dir = LEFT;
                    //std::cout << "Direction left" << std::endl;
                } else {
                    col++;
                    //std::cout << "Down" << std::endl;
                }
                break;
            case LEFT:
                if (row > 0 && grid[col][row-1] == '#') {
                    dir = UP;
                    //std::cout << "Direction up" << std::endl;
                } else {
                    row--;
                    //std::cout << "Left" << std::endl;
                }
                break;
            default:
                break;
        }
        // Check boundaries
        if (col < 0 || col >= rows || row < 0 || row >= cols) {
            std::cout << "Out of bounds at col: " << col << ", row: " << row << std::endl;
            outside = true;
        }
    }

    std::cout << "Origin visits: " << (int)visits[80][32] << std::endl;

    int total = 0;
    for (int i = 0; i < cols; i++) {
        for (int k = 0; k < rows; k++) {
            if (visits[i][k] > 0) {
                std::cout << "o";
                total++;
            } else {
                std::cout << grid[i][k];
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Total visited: " << total << std::endl;
    // 5145
}

bool RunMap(char grid[130][130], char visits[130][130], int cols, int rows, int col, int row, bool visualize_map)
{
    enum direction { UP, RIGHT, DOWN, LEFT };
    direction dir = UP;

    //char visits[cols][rows];
    memset(visits, 0, sizeof(visits));

    std::vector<int> encountered_obstacles;

    bool outside = false;
    bool looping = false;
    while (!outside && !looping) {
        int obs_id = 0;

        // Map visited tiles
        visits[col][row]++;

        // Turn at obstructions or move
        switch (dir) {
            case UP:
                if (col > 0 && grid[col-1][row] == '#') {
                    obs_id = (dir << 16) + (col << 8) + row;
                    dir = RIGHT;
                    //std::cout << "Direction right" << std::endl;
                } else {
                    col--;
                    //std::cout << "Up" << std::endl;
                }
                break;
            case RIGHT:
                if (row < (rows - 1) && grid[col][row+1] == '#') {
                    obs_id = (dir << 16) + (col << 8) + row;
                    dir = DOWN;
                    //std::cout << "Direction down" << std::endl;
                } else {
                    row++;
                    //std::cout << "Right" << std::endl;
                }
                break;
            case DOWN:
                if (col < (cols - 1) && grid[col+1][row] == '#') {
                    obs_id = (dir << 16) + (col << 8) + row;
                    dir = LEFT;
                    //std::cout << "Direction left" << std::endl;
                } else {
                    col++;
                    //std::cout << "Down" << std::endl;
                }
                break;
            case LEFT:
                if (row > 0 && grid[col][row-1] == '#') {
                    obs_id = (dir << 16) + (col << 8) + row;
                    dir = UP;
                    //std::cout << "Direction up" << std::endl;
                } else {
                    row--;
                    //std::cout << "Left" << std::endl;
                }
                break;
            default:
                break;
        }
        // Check boundaries
        if (col < 0 || col >= rows || row < 0 || row >= cols) {
            //std::cout << "Out of bounds at col: " << col << ", row: " << row << std::endl;
            outside = true;
        }
        // Check for looping - have we been here before?
        if (obs_id > 0) {
            if (std::find(encountered_obstacles.begin(), encountered_obstacles.end(), obs_id) != encountered_obstacles.end()) {
                std::cout << "Looping at col: " << col << ", row: " << row << std::endl;
                looping = true;
            } else {
                encountered_obstacles.push_back(obs_id);
            }
        }
    }

    //std::cout << "Origin visits: " << (int)visits[80][32] << std::endl;

    //int total = 0;
    if (visualize_map) {
        for (int i = 0; i < cols; i++) {
            for (int k = 0; k < rows; k++) {
                if (visits[i][k] > 0) {
                    std::cout << "o";
                    //total++;
                } else {
                    std::cout << grid[i][k];
                }
            }
            std::cout << std::endl;
        }
    }

    return looping;
}

void Day6part2()
{
    int col = 0;
    int row = 0;

    std::fstream file("input6.txt");
    const int rows = 130;
    const int cols = 130;
    char grid[cols][rows];
    for (int i = 0; i < cols; i++) {
        std::string input;
        std::getline(file, input);
        for (size_t k = 0; k < input.length(); k++) {
            grid[i][k] = input[k];
            if (input[k] == '^') {
                col = i;
                row = k;
            }
            //std::cout << input[k];
        }
        //std::cout << std::endl;
    }
    //std::cout << std::endl;
    std::cout << "Start at col: " << col << ", row: " << row << std::endl;

    // first run to find visited tiles
    char visits[cols][rows];
    RunMap(grid, visits, cols, rows, col, row, true);

    // Add obstacle at each visited tile in turn and run map
    int total = 0;
    for (int i = 0; i < cols; i++) {
        for (int k = 0; k < rows; k++) {
            //if (visits[i][k] > 0) {
                // Copy grid
                char grid_copy[cols][rows];
                memcpy(grid_copy, grid, sizeof(grid));
                // Add obstacle
                grid_copy[i][k] = '#';
                // Run map
                char visits_copy[cols][rows];
                memset(visits_copy, 0, sizeof(visits_copy));
                if (RunMap(grid_copy, visits_copy, cols, rows, col, row, false)) {
                    total++;
                    std::cout << "Obstacle at col: " << i << ", row: " << k << " results in a loop" << std::endl;
                    //exit(0);
                }
            //}
        }
    }

    std::cout << std::endl;
    std::cout << "Added obstacle results in looping: " << total << std::endl;
    // 1523
}

void main()
{
    //Day6part1();
    Day6part2();
}