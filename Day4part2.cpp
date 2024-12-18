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

    // X-MAS Pattern
    for (size_t r = 1; r < grid.size() - 1; r++) {
        for (size_t c = 1; c < grid[r].length() - 1; c++) {
            // M.S or S.M
            // .A.    .A.
            // M.S    S.M
            std::stringstream left_ss;
            std::stringstream right_ss;
            left_ss << grid[r - 1][c - 1] << grid[r][c] << grid[r + 1][c + 1];
            std::string left = left_ss.str();
            right_ss << grid[r - 1][c + 1] << grid[r][c] << grid[r + 1][c - 1];
            std::string right = right_ss.str();
            if ((left == "MAS" || left == "SAM") && (right == "MAS" || right == "SAM")) {
                tot++;
            }
        }
    }

    std::cout << "Total: " << tot << std::endl;
    // 2029
}
