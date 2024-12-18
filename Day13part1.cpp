#include <fstream>
#include <iostream>
#include <vector>
#include <string>

void main()
{
    std::fstream file("input13.txt");
    std::string line;
    long cost = 0;

    bool done = false;
    while (!done) {
        int a_dx = 0;
        int a_dy = 0;
        int b_dx = 0;
        int b_dy = 0;
        int price_x = 0;
        int price_y = 0;

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
        if (sscanf(line.c_str(), "Prize: X=%d, Y=%d", &price_x, &price_y) != 2) {
            done = true;
            continue;
        }

        // Blank line
        std::getline(file, line);

        std::cout << std::endl;
        std::cout << "Button A: X+" << a_dx << ", Y+" << a_dy << std::endl;
        std::cout << "Button B: X+" << b_dx << ", Y+" << b_dy << std::endl;
        std::cout << "Prize: X=" << price_x << ", Y=" << price_y << std::endl;

        // Solve a linear system using Cramer's rule
        // ax+by=e
        // cx+dy=f
        //double a = a_dx, b = b_dx, e = price_x;
        int a = a_dx, b = b_dx, e = price_x;
        //double c = a_dy, d = b_dy, f = price_y;
        int c = a_dy, d = b_dy, f = price_y;

        std::cout << a << "*x + " << b << "*y = " << e << std::endl;
        std::cout << c << "*x + " << d << "*y = " << f << std::endl;

        // Cramer's rule
        int determinant = a*d - b*c;
        if (determinant != 0) {
            int x = (e*d - b*f)/determinant;
            int y = (a*f - e*c)/determinant;
            std::cout << "X=" << x << ", Y=" << y << std::endl;

            int button_a_presses = x;
            int button_b_presses = y;

            int total_x = button_a_presses * a_dx + button_b_presses * b_dx;
            int total_y = button_a_presses * a_dy + button_b_presses * b_dy;

            std::cout << "Total X=" << total_x << ", Y=" << total_y << std::endl;
            if (total_x != price_x || total_y != price_y) {
                // Only a non-integer solution is possible
                std::cout << "Error in calculation" << std::endl;
            } else {
                cost += 3 * button_a_presses + button_b_presses;
                std::cout << "Cost: " << (3 * button_a_presses + button_b_presses) << std::endl;
            }
        } else {
            std::cout << "No solution" << std::endl;
        }
    }

    std::cout << "Total cost: " << cost << std::endl;
    // 40069
}