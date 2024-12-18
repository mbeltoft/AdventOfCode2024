#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <inttypes.h>

void main()
{
    std::fstream file("input13.txt");
    std::string line;
    int64_t cost = 0;

    bool done = false;
    while (!done) {
        int a_dx = 0;
        int a_dy = 0;
        int b_dx = 0;
        int b_dy = 0;
        int64_t price_x = 0;
        int64_t price_y = 0;

        // Button A: X+94, Y+34
        std::getline(file, line);
        if (sscanf(line.c_str(), "Button A: X+%d, Y+%d", &a_dx, &a_dy) != 2) {
            done = true;
            continue;
        }

        // Button B: X+22, Y+67
        std::getline(file, line);
        if (sscanf(line.c_str(), "Button B: X+%d, Y+%d", &b_dx, &b_dy) != 2) {
            done = true;
            continue;
        }

        // Prize: X=8400, Y=5400
        std::getline(file, line);
        if (sscanf(line.c_str(), "Prize: X=%lld, Y=%lld", &price_x, &price_y) != 2) {
            done = true;
            continue;
        }

        price_x += 10000000000000LL;
        price_y += 10000000000000LL;

        // Blank line
        std::getline(file, line);

        //std::cout << std::endl;
        //std::cout << "Button A: X+" << a_dx << ", Y+" << a_dy << std::endl;
        //std::cout << "Button B: X+" << b_dx << ", Y+" << b_dy << std::endl;
        //std::cout << "Prize: X=" << price_x << ", Y=" << price_y << std::endl;

        // Solve a linear system using Cramer's rule
        // ax+by=e
        // cx+dy=f
        int64_t a = a_dx, b = b_dx, e = price_x;
        int64_t c = a_dy, d = b_dy, f = price_y;

        //std::cout << a << "*x + " << b << "*y = " << e << std::endl;
        //std::cout << c << "*x + " << d << "*y = " << f << std::endl;

        // Cramer's rule
        int64_t determinant = a*d - b*c;
        if (determinant != 0) {
            int64_t x = (e*d - b*f)/determinant;
            int64_t y = (a*f - e*c)/determinant;
            //std::cout << "X=" << x << ", Y=" << y << std::endl;

            int64_t button_a_presses = x;
            int64_t button_b_presses = y;

            int64_t total_x = button_a_presses * a_dx + button_b_presses * b_dx;
            int64_t total_y = button_a_presses * a_dy + button_b_presses * b_dy;

            //std::cout << "Total X=" << total_x << ", Y=" << total_y << std::endl;
            if (total_x != price_x || total_y != price_y) {
                // Only a non-integer solution is possible
                //std::cout << "Error in calculation" << std::endl;
            } else {
                cost += 3 * button_a_presses + button_b_presses;
                //std::cout << "Cost: " << (3 * button_a_presses + button_b_presses) << std::endl;
            }
        } else {
            //std::cout << "No solution" << std::endl;
        }
    }

    std::cout << "Total cost: " << cost << std::endl;
    // 71493195288102
}