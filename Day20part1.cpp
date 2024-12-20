#include <iostream>
#include <fstream>
#include <cstdint>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <vector>
#include <map>
#include <string>

// https://github.com/mquig42/AdventOfCode2024/tree/main
// https://github.com/mquig42/AdventOfCode2024/blob/main/src/day16.cpp

struct State
{
    uint16_t position;
    uint8_t direction;

    bool operator==(State s) const
    {
        return this->position == s.position && this->direction == s.direction;
    }

    size_t operator()(State s) const
    {
        return s.position;
    }
};

struct QueueNode
{
    uint16_t position;
    uint8_t direction;
    uint32_t score;
    std::unordered_set<uint16_t> path;
    bool operator<(const QueueNode &b) const
    {
        return score > b.score; //Easiest way to make queue give minimum value instead of maximum
    }
};

const uint8_t up = 0;
const uint8_t right = 1;
const uint8_t down = 2;
const uint8_t left = 3;
const uint16_t step_penalty = 1; // Was 1 on Day16
const uint16_t turn_penalty = 1000; // Was 1000 on Day16

std::unordered_set<uint16_t> spaces;
std::unordered_set<uint16_t> allPaths;
uint64_t bestScore = UINT64_MAX;
uint64_t orgBestScore = UINT64_MAX;
uint16_t start, end;

uint16_t makeCoord(uint8_t row, uint8_t col)
{
    return row * 256 + col;
}

uint8_t row(uint16_t coord)
{
    return coord / 256;
}

uint8_t col(uint16_t coord)
{
    return coord % 256;
}

const int Y_SIZE = 15;
const int X_SIZE = 15;
char orgMap[Y_SIZE][X_SIZE];
char map[Y_SIZE][X_SIZE];

void load(const std::string &filename)
{
    std::string line;
    uint8_t row = 0;

    std::ifstream file(filename);
    while (getline(file, line)) {
        for(size_t i = 0; i < line.length(); i++) {
            if(line[i] == 'S') {
                start = makeCoord(row, i);
                orgMap[row][i] = '.';
            }
            else if(line[i] == 'E') {
                end = makeCoord(row, i);
                orgMap[row][i] = '.';
            }
            else if(line[i] == '.') {
                orgMap[row][i] = '.';
            }
            else {
                orgMap[row][i] = line[i];
            }
        }
        row++;
    }
}

void copyMap()
{
    for (size_t y = 0; y < Y_SIZE; y++) {
        for(size_t x = 0; x < X_SIZE; x++) {
            map[y][x] = orgMap[y][x];
        }
    }
}

void displayMap()
{
    for (size_t y = 0; y < Y_SIZE; y++) {
        for(size_t x = 0; x < X_SIZE; x++) {
            std::cout << map[y][x];
        }
        std::cout << std::endl;
    }
}

void mapSpaces()
{
    spaces.clear();
    for (size_t y = 0; y < Y_SIZE; y++) {
        for(size_t x = 0; x < X_SIZE; x++) {
            if (map[y][x] == '.') {
                spaces.insert(makeCoord(y, x));
            }
        }
    }
    //std::cout << "Spaces: " << spaces.size() << std::endl;
}

// Dijkstra's algorithm, set to run until all possible paths have been explored
// It stores the length of the shortest path, and a set of all points on any shortest path
// in the class vars bestScore and allPaths
void runDijkstra()
{
    uint16_t nextPos;
    uint8_t nextDir;
    QueueNode current;
    std::unordered_map<State, uint32_t, State> visited;
    std::priority_queue<QueueNode> unvisited;

    bestScore = UINT64_MAX;
    allPaths.clear();
    
    visited[{start, 1}] = 0;
    unvisited.push({ start, 1, 0 });

    while (!unvisited.empty())
    {
        current = unvisited.top();
        unvisited.pop();

        std::unordered_set<uint16_t> path(current.path);
        path.insert(current.position);

        if (current.position == end && current.score <= bestScore)
        {
            bestScore = current.score;
            for (uint16_t p : path)
                allPaths.insert(p);
        }

        // From any state there are 3 possible moves:
        // Rotate left, rotate right, and go forward
        // nextPos represents the position directly in front
        if (current.direction == up)
            nextPos = makeCoord(row(current.position) - 1, col(current.position));
        else if (current.direction == right)
            nextPos = makeCoord(row(current.position), col(current.position) + 1);
        else if (current.direction == down)
            nextPos = makeCoord(row(current.position) + 1, col(current.position));
        else
            nextPos = makeCoord(row(current.position), col(current.position) - 1);

        // If nextPos is a valid space and hasn't already been reached by a shorter path, add it to unvisited
        if (spaces.count(nextPos) && (visited.count({ nextPos, current.direction }) == 0 || visited[{nextPos, current.direction}] >= (current.score + step_penalty)))
        {
            unvisited.push({ nextPos, current.direction, current.score + step_penalty, path });
            visited[{nextPos, current.direction}] = current.score + step_penalty;
        }

        // Turn right
        nextDir = (current.direction + 1) % 4;
        if (visited.count({ current.position, nextDir }) == 0 || visited[{current.position, nextDir}] >= (current.score + turn_penalty))
        {
            unvisited.push({ current.position, nextDir, current.score + turn_penalty, path });
            visited[{current.position, nextDir}] = current.score + turn_penalty;
        }

        // Turn left
        nextDir = (current.direction + 3) % 4;
        if (visited.count({ current.position, nextDir }) == 0 || visited[{current.position, nextDir}] >= (current.score + turn_penalty))
        {
            unvisited.push({ current.position, nextDir, current.score + turn_penalty, path });
            visited[{current.position, nextDir}] = current.score + turn_penalty;
        }
    }
}

void main()
{
    load("input20test1.txt");
    copyMap();
    //displayMap();
    mapSpaces();

    // Run Dijkstra
    runDijkstra();

    // Save base result
    orgBestScore = bestScore % 1000; // Can't get rid of turn_penalty
    std::cout << "Org steps: " << orgBestScore << std::endl;

    std::vector<uint64_t> scores;

    // Run through inner map one tile at a time
    for (int y = 1; y < Y_SIZE - 1; y++) {
        for (int x = 1; x < X_SIZE - 2; x++) {
            // If '#' replace it with '.' as a cheat
            if (map[y][x] == '#') {
                map[y][x] = '.';
                //std::cout << "1: " << y << "," << x << std::endl;

                // Run through rest of inner map to replace one more '#'
                bool done = false;
                for (int yn = y; yn < Y_SIZE - 1 && !done; yn++) {
                    for (int xn = x +  1; xn < X_SIZE - 1 && !done; xn++) {
                        // If '#' replace it with '.' as a cheat
                        if (map[yn][xn] == '#') {
                            map[yn][xn] = '.';
                            //std::cout << "2: " << yn << "," << xn << std::endl;

                            // Map spaces
                            mapSpaces();

                            // Run Dijkstra
                            runDijkstra();

                            // Put result in a vector
                            if (bestScore != UINT64_MAX) {
                                bestScore = (bestScore % 1000);
                                //std::cout << "Steps: " << bestScore << std::endl;
                                scores.push_back(bestScore);
                            }

                            done = true;
                        }
                    }
                }

                // Copy map
                copyMap();
            }
        }
    }

    // Count results with same number of steps
    std::map<uint64_t, uint64_t> counts;
    for (auto score : scores) {
        counts[orgBestScore - score]++;
    }

    for (auto count : counts) {
        if (count.first > 0) {
            std::cout << "There are " << count.second << " cheats that save " << count.first << " picoseconds." << std::endl;
            //std::cout << "Saved steps: " << count.first << " count: " << count.second << std::endl;
        }
    }

    // How many cheats would save at least 100? Compared to original?
}
