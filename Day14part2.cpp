#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <conio.h>

struct Robot
{
public:
    Robot(int x, int y, int vx, int vy) : x(x), y(y), vx(vx), vy(vy) {};
    int x = 0;
    int y = 0;
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
    //std::cout << "Robots: " << robots.size() << std::endl;

    const int x_size = 101;
    const int y_size = 103;

    // Run simulation
    for (int i = 0; i < 10000; i++) {
        for (auto &robot : robots) {
            robot.x = (x_size + robot.x + robot.vx) % x_size;
            robot.y = (y_size + robot.y + robot.vy) % y_size;
        }

        // Heat map for robots
        int map[y_size][x_size];
        memset(map, 0, sizeof(map));
        for (auto robot : robots) {
            map[robot.y][robot.x]++;
        }

        // Generate a string per line
        bool visualize_map = false;
        for (int y = 0; y < y_size; y++) {
            std::stringstream ss;
            for (int x = 0; x < x_size; x++) {
                if (map[y][x]) {
                    ss << "#";
                } else {
                    ss << ".";
                }
            }
            // See if robots line up - could be a Christmas tree
            if (ss.str().find("##########") != std::string::npos) {
                visualize_map = true;
                break;
            }
        }

        if (visualize_map) {
            std::cout << "Iteration: " << (int)(i + 1) << std::endl; 
            for (int y = 0; y < y_size; y++) {
                for (int x = 0; x < x_size; x++) {
                    if (map[y][x]) {
                        std::cout << "#";
                    } else {
                        std::cout << ".";
                    }
                }
                std::cout << std::endl;
            }
            //getch();
        }
    }
    // 6398
}
