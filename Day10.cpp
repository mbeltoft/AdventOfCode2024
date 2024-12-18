#include <vector>
#include <string>
#include <fstream>
#include <iostream>

struct Point
{
public:
    Point(int r, int c) : r(r), c(c) {};
    int r = 0;
    int c = 0;
};

struct Trailhead
{
    Trailhead(int r, int c) : r(r), c(c) {};
    int r = 0;
    int c = 0;
    std::vector<Point> peaks;
    int trails_to_the_top = 0;
};

#define COLS 45
#define ROWS 45

bool MapWalk(char map[ROWS][COLS], Trailhead &trailhead, int row, int col, int height)
{
    // Are we at the top?
    if (height >= 9) {
        //std::cout << "Top at row: " << row << ", col: " << col << std::endl;
        trailhead.trails_to_the_top++;
        for (auto peak : trailhead.peaks) {
            if (peak.r == row && peak.c == col) {
                return false; // Already visited this peak
            }
        }
        // One more peak reached from this trailhead
        trailhead.peaks.push_back(Point(row, col));
        return true;
    }

    // Look up
    if (row > 0 && map[row-1][col] == height + 1) {
        //std::cout << "up to " << height + 1 << std::endl;
        MapWalk(map, trailhead, row-1, col, height + 1);
    }
    // Look right
    if (col < COLS - 1 && map[row][col+1] == height + 1) {
        //std::cout << "right to " << height + 2 << std::endl;
        MapWalk(map, trailhead, row, col+1, height + 1);
    }
    // Look down
    if (row < ROWS - 1 && map[row+1][col] == height + 1) {
        //std::cout << "down to " << height + 1 << std::endl;
        MapWalk(map, trailhead, row+1, col, height + 1);
    }
    // Look left
    if (col > 0 && map[row][col-1] == height + 1) {
        //std::cout << "left to " << height + 1 << std::endl;
        MapWalk(map, trailhead, row, col-1, height + 1);
    }
    //std::cout << "Dead end at height " << height << std::endl;
    return false;
}

void part1()
{
    int row = 0;
    std::fstream file("input10.txt");
    std::string line;
    char map[ROWS][COLS];
    std::vector<Trailhead> trailheads;

    // Load map and find trailheads
    while (std::getline(file, line)) {
        for (int c = 0; c < line.size(); c++) {
            if (line[c] == '.') {
                map[row][c] = -1;
            } else {
                map[row][c] = line[c] - '0';
            }
            if (line[c] == '0') {
                trailheads.push_back(Trailhead(row, c));
            }
        }
        row++;
    }
    std::cout << "Trailheads: " << trailheads.size() << std::endl;

    // Start walking from each trailhead
    for (auto &trailhead : trailheads) {
        //std::cout << "Trailhead at row: " << trailhead.r << ", col: " << trailhead.c << std::endl;
        MapWalk(map, trailhead, trailhead.r, trailhead.c, 0);
    }

    int total_score = 0;
    for (auto trailhead : trailheads) {
        //std::cout << "Trailhead at " << trailhead.r << "," << trailhead.c << " score: " << trailhead.peaks.size() << std::endl;
        total_score += trailhead.peaks.size();
        //for (auto peak : trailhead.peaks) {
        //    std::cout << "Peak at " << peak.r << "," << peak.c << std::endl;
        //}
    }
    std::cout << "Total score: " << total_score << std::endl;
    // 574
}

void part2()
{
    int row = 0;
    std::fstream file("input10.txt");
    std::string line;
    char map[ROWS][COLS];
    std::vector<Trailhead> trailheads;

    // Load map and find trailheads
    while (std::getline(file, line)) {
        for (int c = 0; c < line.size(); c++) {
            map[row][c] = line[c] - '0';
            if (line[c] == '0') {
                trailheads.push_back(Trailhead(row, c));
            }
        }
        row++;
    }
    std::cout << "Trailheads: " << trailheads.size() << std::endl;

    // Start walking from each trailhead
    for (auto &trailhead : trailheads) {
        //std::cout << "Trailhead at row: " << trailhead.r << ", col: " << trailhead.c << std::endl;
        MapWalk(map, trailhead, trailhead.r, trailhead.c, 0);
    }

    int total_score = 0;
    int total_rating = 0;
    for (auto trailhead : trailheads) {
        //std::cout << "Trailhead at " << trailhead.r << "," << trailhead.c << " score: " << trailhead.peaks.size() << std::endl;
        total_score += trailhead.peaks.size();
        total_rating += trailhead.trails_to_the_top;
        //for (auto peak : trailhead.peaks) {
        //    std::cout << "Peak at " << peak.r << "," << peak.c << std::endl;
        //}
    }
    std::cout << "Total score: " << total_score << std::endl; // part 1
    // 574
    std::cout << "Total rating: " << total_rating << std::endl; // part 2
    // 1238
}

void main()
{
    //part1();
    part2();
}
