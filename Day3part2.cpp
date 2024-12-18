#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>

void main()
{
    std::ifstream file("input3.txt");
    std::string line;
    long result = 0;
    std::regex re(R"(^mul\((\d+),(\d+)\))");
    bool enabled = true;

    while (std::getline(file, line)) {
        size_t pos = 0;
        std::string segment;
        while (pos < line.size()) {
            if (line.substr(pos, 7) == "don't()") {
                // enable and go up 7 positions for "don't()"
                enabled = false;
                pos += 7;
            } else if (line.substr(pos, 4) == "do()") {
                // enable and go up 4 positions for "do()"
                enabled = true;
                pos += 4;
            } else if (line.substr(pos, 4) == "mul(") {
                if (enabled) {
                    std::smatch match;
                    // search only the segment substring starting from "pos" to the end of the line
                    segment = line.substr(pos);
                    if (std::regex_search(segment, match, re)) {
                        int a = std::stoi(match[1]);
                        int b = std::stoi(match[2]);
                        result += a * b;
                        pos += match[0].length();
                    } else {
                        ++pos;
                    }
                } else {
                    pos += 4;
                    // jump only up the same length as the match "mul("
                }
            } else {
                ++pos;
                // go up one position as nothing was matched
            }
        }
    }

    std::cout << result << std::endl;
    // 98729041
}
