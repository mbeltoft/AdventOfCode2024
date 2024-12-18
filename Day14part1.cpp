#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Robot
{
public:
    Robot(int px, int py, int vx, int vy) : px(px), py(py), vx(vx), vy(vy) {};
    int px = 0;
    int py = 0;
    int vx = 0;
    int vy = 0;
};
std::vector<Robot> robots;

void main()
{
    std::fstream file("input14.txt");
    std::string line;
    int64_t cost = 0;

    // p=0,4 v=3,-3
    while (std::getline(file, line)) {
        int px = 0;
        int py = 0;
        int vx = 0;
        int vy = 0;
        if (sscanf(line.c_str(), "p=%d,%d v=%d,%d", &px, &py, &vx, &vy) == 4) {
            robots.push_back(Robot(px, py, vx, vy));
        }
    }
    std::cout << "Robots: " << robots.size() << std::endl;

    const int x_size = 101; // 11;
    const int y_size = 103; // 7;

    // Run simulation
    for (int i = 0; i < 100; i++) {
        for (auto &robot : robots) {
            int x = (x_size + robot.px + robot.vx) % x_size;
            int y = (y_size + robot.py + robot.vy) % y_size;
            //std::cout << robot.px << "->" << x << " " << robot.py << "->" << y << std::endl;
            robot.px = x;
            robot.py = y;
        }
    }

    // Heat map for robots
    int map[y_size][x_size];
    memset(map, 0, sizeof(map));
    for (auto robot : robots) {
        map[robot.py][robot.px]++;
    }

    // Count robots in quarters, skipping middle vertical and horizontal line
    int x_middle = x_size / 2;
    int y_middle = y_size / 2;
    int q1 = 0;
    int q2 = 0;
    int q3 = 0;
    int q4 = 0;

    int sum = 0;
    for (int y = 0; y < y_size; y++) {
        for (int x = 0; x < x_size; x++) {
            if (x < x_middle && y < y_middle) q1 += map[y][x];
            if (x > x_middle && y < y_middle) q2 += map[y][x];
            if (x < x_middle && y > y_middle) q3 += map[y][x];
            if (x > x_middle && y > y_middle) q4 += map[y][x];
            sum += map[y][x];
            std::cout << std::hex << map[y][x];
        }
        std::cout << std::dec << std::endl;
    }

    //std::cout << "Sum: " << sum << std::endl; // Just to make sure all 500 robots are still on the map
    std::cout << (int)(q1 * q2 * q3 * q4) << std::endl;
    // 219512160
}
