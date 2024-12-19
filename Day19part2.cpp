#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <utility>
#include <cstdint>
#include "StringSplit.h"

// https://github.com/mkinkela1/advent-of-code/blob/master/2024/day-19/solution.cpp

std::vector<std::pair<size_t, char>> dead_ends;

uint64_t WaysToCreate(const std::string& design, const std::vector<std::string>& patterns)
{
    size_t n = design.size();
    std::vector<uint64_t> dp(n + 1, 0);
    dp[0] = 1;

    for (int i = 1; i <= n; i++) {
        for (const auto &pattern : patterns) {
            size_t len = pattern.size();
            if (i >= len && design.substr(i - len, len) == pattern) {
                dp[i] += dp[i - len];
            }
        }
    }

    return dp[n];
}

void main()
{
    std::string line;
    std::vector<std::string> patterns;
    std::vector<std::string> designs;

    std::ifstream file("input19.txt");
    // Patterns in first line
    getline(file, line);
    patterns = StringSplit(line, ", ");
    getline(file, line); // Read empty line
    // Designs in the remaining lines
    while (getline(file, line)) {
        designs.push_back(line);
    }
    std::cout << "Patterns: " << patterns.size() << std::endl;
    std::cout << "Designs: " << designs.size() << std::endl;

    uint64_t total = 0;
    for (const auto& design : designs) {
        total += WaysToCreate(design, patterns);
    }
    std::cout << "Total: " << total << std::endl;
    // 771745460576799
}
