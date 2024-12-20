#include <iostream>
#include <fstream>
#include <cstdint>
#include <unordered_set>
#include <unordered_map>
#include <queue>
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

uint8_t up = 0;
uint8_t right = 1;
uint8_t down = 2;
uint8_t left = 3;

std::unordered_set<uint16_t> spaces;
std::unordered_set<uint16_t> allPaths;
uint64_t bestScore = UINT64_MAX;
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

void load(const std::string &filename)
{
    std::string line;
    uint8_t row = 0;

    std::ifstream file(filename);
    while (getline(file, line)) {
        for(size_t i = 0; i < line.length(); i++) {
            if(line[i] == 'S') {
                start = makeCoord(row, i);
                spaces.insert(makeCoord(row, i));
            }
            else if(line[i] == 'E') {
                end = makeCoord(row, i);
                spaces.insert(makeCoord(row, i));
            }
            else if(line[i] == '.') {
                spaces.insert(makeCoord(row, i));
            }
        }
        row++;
    }
}

//Dijkstra's algorithm, set to run until all possible paths have been explored
//It stores the length of the shortest path, and a set of all points on any shortest path
//in the class vars bestScore and allPaths
void runDijkstra()
{
    uint16_t nextPos;
    uint8_t nextDir;
    QueueNode current;
    std::unordered_map<State, uint32_t, State> visited;
    std::priority_queue<QueueNode> unvisited;

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
        if (spaces.count(nextPos) && (visited.count({ nextPos, current.direction }) == 0 || visited[{nextPos, current.direction}] >= (current.score + 1)))
        {
            unvisited.push({ nextPos, current.direction, current.score + 1, path });
            visited[{nextPos, current.direction}] = current.score + 1;
        }

        // Turn right
        nextDir = (current.direction + 1) % 4;
        if (visited.count({ current.position, nextDir }) == 0 || visited[{current.position, nextDir}] >= (current.score + 1000))
        {
            unvisited.push({ current.position, nextDir, current.score + 1000, path });
            visited[{current.position, nextDir}] = current.score + 1000;
        }

        // Turn left
        nextDir = (current.direction + 3) % 4;
        if (visited.count({ current.position, nextDir }) == 0 || visited[{current.position, nextDir}] >= (current.score + 1000))
        {
            unvisited.push({ current.position, nextDir, current.score + 1000, path });
            visited[{current.position, nextDir}] = current.score + 1000;
        }
    }
}

uint64_t solve1()
{
    runDijkstra();
    return bestScore;
}

uint64_t solve2()
{
    return allPaths.size();
}

void main()
{
    load("input16.txt");
    auto bestScore = solve1();
    std::cout << "Best score: " << bestScore << std::endl;
    // 135512

    auto numPaths = solve2();
    std::cout << "Number of paths: " << numPaths << std::endl;
    // 541
}
