#include <string>
#include <iostream>
#include <fstream>

#define FILENAME "input20.txt"
//#define FILENAME "input20test1.txt"
#define SIZE 141
//#define SIZE 15
#define CUTOFF 100
//#define CUTOFF 1

int main() {
    // Start column, row
    int sc = 0;
    int sr = 0;

    // End column, row
    int ec = 0;
    int er = 0;

    char map[SIZE][SIZE];
    int dist[SIZE][SIZE]; // Distance (number of steps) from start

    std::fstream file(FILENAME);
    std::string line;
    int r = 0;
    while (std::getline(file, line)) {
        for (size_t c = 0; c < line.size(); c++) {
            dist[c][r] = -1;
            map[c][r] = line[c];
            if (map[c][r] == 'S') {
                sr = r;
                sc = c;
            }
            if (map[c][r] == 'E') {
                er = r;
                ec = c;
            }
        }
        r++;
    }

    int c = sc;
    r = sr;
    int d = 0;

    // Map is guaranteed to not have forks, pathfinding is not required, just go forward avoiding walls
    while (c != ec || r != er) {
        dist[c][r] = d;
        if (r - 1 >= 0 && map[c][r - 1] != '#' && dist[c][r - 1] == -1) {
            r--; // Go left
        }
        else if (c + 1 < SIZE && map[c + 1][r] != '#' && dist[c + 1][r] == -1) {
            c++; // Go down
        }
        else if (r + 1 < SIZE && map[c][r + 1] != '#' && dist[c][r + 1] == -1) {
            r++; // Go right
        }
        else if (c - 1 >= 0 && map[c - 1][r] != '#' && dist[c - 1][r] == -1) {
            c--; // Go up
        }
        d++; // One step further away from start
    }
    dist[ec][er] = d;
    // dist now contains the route walked with increasing numbers towards the end
    //printf("Dist: %d\n", d);

    // Check for cheats
    // A cheat is a .#. pattern horizontally or vertically where . can also be S or E
    int num_cheats = 0;
    for (int c = 0; c < SIZE; c++) {
        for (int r = 0; r < SIZE; r++) {
            // Search down
            if (c + 2 < SIZE && map[c][r] != '#' && map[c + 1][r] == '#' && map[c + 2][r] != '#') {
                int saved;
                // What's the distance saved if cheat was used?
                saved = abs(dist[c + 2][r] - dist[c][r]) - 2; // 2 for walking the cheat
                if (saved >= CUTOFF) {
                    num_cheats++;
                }
            }
            // Search right
            if (r + 2 < SIZE && map[c][r] != '#' && map[c][r + 1] == '#' && map[c][r + 2] != '#') {
                int saved;
                // What's the distance saved if cheat was used?
                saved = abs(dist[c][r + 2] - dist[c][r]) - 2; // 2 for walking the cheat
                if (saved >= CUTOFF) {
                    num_cheats++;
                }
            }
        }
    }
    std::cout << "Number of cheats that would save " << CUTOFF << " or more: " << num_cheats << std::endl;
    // 1463
}