#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

void main()
{
    std::vector<std::string> grid;
    std::string l;
    std::fstream file("input4.txt");
    while (std::getline(file, l)) {
        grid.push_back(l);
    }

    long tot = 0;

    // Rows
    for (auto s : grid) {
        for (size_t i = 0; i < s.length() - 3; i++) {
            auto cut = s.substr(i, 4);

            if (cut == "XMAS" || cut == "SAMX") {
                tot++;
            }
        }
    }

    // Cols
    for (size_t i = 0; i < grid[0].length(); i++) {
        for (size_t j = 0; j < grid.size() - 3; j++) {
            std::stringstream ss;
            ss << grid[j][i] << grid[j+1][i] << grid[j+2][i] << grid[j+3][i];

            if (ss.str() == "XMAS" || ss.str() == "SAMX") {
                tot++;
            }
        }
    }

    // Left diagonal
    for (size_t r = 0; r < grid.size() - 3; r++) {
        for (size_t i = 3; i < grid[r].length(); i++) {
            std::stringstream ss;
            ss << grid[r][i] << grid[r + 1][i - 1] << grid[r + 2][i - 2] << grid[r + 3][i - 3];
            if (ss.str() == "XMAS" || ss.str() == "SAMX") {
                tot++;
            }
        }
    }

    // Right diagonal
    for (size_t r = 0; r < grid.size() - 3; r++) {
        for (size_t i = 0; i < grid[r].length() - 3; i++) {
            std::stringstream ss;
            ss << grid[r][i] << grid[r + 1][i + 1] << grid[r + 2][i + 2] << grid[r + 3][i + 3];
            if (ss.str() == "XMAS" || ss.str() == "SAMX") {
                tot++;
            }
        }
    }

    std::cout << "Total: " << tot << std::endl;
    // 2567
}