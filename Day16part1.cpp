#include <fstream>
#include <iostream>
#include <string>
#include <random>
#include <stdio.h>

// Try this instead
// https://github.com/JDSherbert/A-Star-Pathfinding/blob/main/Pathfinder.cpp

const int COLS = 15;
const int ROWS = 15;
char map[ROWS][COLS];

const int mazeRows = ROWS;
const int mazeColumns = COLS;
#if 0
char maze[mazeRows][mazeColumns + 1] = {
    "# #######",
    "#   #   #",
    "# ### # #",
    "# #   # #",
    "# # # ###",
    "#   # # #",
    "# ### # #",
    "#   #   #",
    "####### #",
};
#endif
const char wall = '#';
const char path = '.';
const char someDude = '*';

class COORD
{
public:
    int x;
    int y;
    COORD(int _x = 0, int _y = 0) { x = _x, y = _y; }
    COORD(const COORD &coord) { x = coord.x; y = coord.y; }
};
COORD startingPoint(1, 0); // Overwritten during load
COORD endingPoint(7, 8); // Overwritten during load

void loadMaze()
{
    std::fstream file("input16test1.txt");
    std::string line;
    int row = 0;
    int rx = 0;
    int ry = 0;
    int ex = 0;
    int ey = 0;

    // Load map and find start position S and exit position E
    while (std::getline(file, line)) {
        for (size_t c = 0; c < line.size(); c++) {
            map[row][c] = line[c];
            if (line[c] == 'S') {
                rx = c;
                ry = row;
                map[row][c] = '.';
            }
            if (line[c] == 'E') {
                ex = c;
                ey = row;
                map[row][c] = '.';
            }
        }
        row++;
    }

    startingPoint.x = rx;
    startingPoint.y = ry;
    endingPoint.x = ex;
    endingPoint.y = ey;
}

void printDaMaze() {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            if (r == startingPoint.y && c == startingPoint.x) {
                std::cout << "S";
            } else if (r == endingPoint.y && c == endingPoint.x) {
                std::cout << "E";
            } else {
                std::cout << map[r][c];
            }
        }
        std::cout << std::endl;
    }
#if 0
    for (int Y = 0; Y < mazeRows; Y++) {
        printf("%s\n", maze[Y]);
    }
    printf("\n");
#endif
}

char get(int x, int y) {
    if (x < 0 || x >= mazeColumns || y < 0 || y >= mazeRows) return wall;
    return map[y][x];
}

void set(int x, int y, char ch) {
    if (x < 0 || x >= mazeColumns || y < 0 || y >= mazeRows) return;
    map[y][x] = ch;
}

bool solve(int x, int y) {
    // Make the move (if it's wrong, we will backtrack later. To illustrate, I have a version of the below in Javascript)
    set(x, y, someDude);

    // If you want progressive update, uncomment these lines...
    //printDaMaze();
    //sleep(50);
    // Check if we have reached our goal.
    if (x == endingPoint.x && y == endingPoint.y) return true;
    // Recursively search for our goal.
    if (get(x - 1, y) == path && solve(x - 1, y)) return true;
    if (get(x + 1, y) == path && solve(x + 1, y)) return true;
    if (get(x, y - 1) == path && solve(x, y - 1)) return true;
    if (get(x, y + 1) == path && solve(x, y + 1)) return true;
    // Otherwise we need to backtrack and find another solution.
    set(x, y, path);
    // If you want progressive update, uncomment these lines...
    //printDaMaze();
    //sleep(50);
    return false;
}

void main() {
    loadMaze();
    printDaMaze();
    if (solve(startingPoint.x, startingPoint.y)) {
        printDaMaze();
        printf("Solved!\n");
    } else {
        printf("Cannot solve. :-(\n");
    }
}


void something()
{
    int row = 0;
    std::fstream file("input16test1.txt");
    std::string line;
    const int COLS = 15;
    const int ROWS = 15;
    char map[ROWS][COLS];
    int rx = 0;
    int ry = 0;
    enum direction { N, E, S, W };
    direction dir = E;
    int64_t total_score = 0;

    // Load map and find trailheads
    while (std::getline(file, line)) {
        for (size_t c = 0; c < line.size(); c++) {
            map[row][c] = line[c];
            if (line[c] == 'S') {
                rx = c;
                ry = row;
            }
        }
        row++;
    }

    for (int r = 0; r < 1000; r++) {
        bool done = false;
        do {
#if 0
            if (ry == 9 && rx == 3) {
                std::cout << "Found the point" << std::endl;
            }
#endif
            bool wall_ahead = false;
            do {
                // Hit a wall and need to change direction?
                switch (dir) {
                    case N:
                        wall_ahead = (map[ry - 1][rx] == '#');
                        break;
                    case E:
                        wall_ahead = (map[ry][rx + 1] == '#');
                        break;
                    case S:
                        wall_ahead = (map[ry + 1][rx] == '#');
                        break;
                    case W:
                        wall_ahead = (map[ry][rx - 1] == '#');
                        break;
                }
                if (wall_ahead) {
                    bool can_go_N = (map[ry - 1][rx] != '#');
                    bool can_go_E = (map[ry][rx + 1] != '#');
                    bool can_go_S = (map[ry + 1][rx] != '#');
                    bool can_go_W = (map[ry][rx - 1] != '#');

                    bool new_direction_found = false;
                    do {


                    } while (!new_direction_found);


                }


            } while (!wall_ahead);

            // Forward is free, move 1 ahead
            switch (dir) {
                case N:
                    ry--;
                break;
                case E:
                    rx++;
                break;
                case S:
                    ry++;
                break;
                case W:
                    rx--;
                break;
            }
            total_score++;
#if 0
            // What's the situation now?
            bool free_N = (map[ry - 1][rx] != '#');
            bool free_E = (map[ry][rx + 1] != '#');
            bool free_S = (map[ry + 1][rx] != '#');
            bool free_W = (map[ry][rx - 1] != '#');
            // Prefer left turns
            switch (dir) {
                case N:
                    if ((free_W && free_E) || (free_W && free_N)) {
                        dir = W;
                        total_score += 1000;
                    }
                    break;
                case E:
                    if ((free_N && free_S) || (free_N && free_E)) {
                        dir = N;
                        total_score += 1000;
                    }
                    break;
                case S:
                    if ((free_E && free_W) || (free_E && free_S)) {
                        dir = E;
                        total_score += 1000;
                    }
                    break;
                case W:
                    if ((free_S && free_N) || (free_S && free_W)) {
                        dir = S;
                        total_score += 1000;
                    }
                    break;
            }

            // Forward is free, move 1 ahead
            switch (dir) {
                case N:
                    ry--;
                    break;
                case E:
                    rx++;
                    break;
                case S:
                    ry++;
                    break;
                case W:
                    rx--;
                    break;
            }
            total_score++;
#endif
            if (map[ry][rx] == 'E') {
                done = true;
            }

            std::cout << ry << "," << rx << " " << total_score << std::endl;
        } while (!done);
    }
}
