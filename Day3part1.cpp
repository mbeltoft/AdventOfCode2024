#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>

void main()
{
    long answer = 0;
    std::regex pattern("^mul\\(([0-9]+),([0-9]+)\\)");
    std::ifstream file("input3.txt");
    std::string line;
    while (getline(file, line)) {
        size_t start_pos = 0;
        do {
            // Search for "mul" in the line
            start_pos = line.find("mul(");
            if (start_pos != std::string::npos) {
                // Cut off the beginning of the line
                line = line.substr(start_pos);
                //std::cout << line.size() << std::endl;

                size_t end_pos = line.find(")");
                if (end_pos != std::string::npos) {
                    std::string work = line.substr(0, end_pos + 1);
                    //std::cout << "Work: " << work << std::endl;

                    std::smatch m;
                    if (std::regex_match(work, m, pattern)) {
                        if (m.size() == 3) {
                            std::cout << "Match " << m[0] << " " << m[1] << " " << m[2] << std::endl;
                            // Convert m[1] to int
                            int num1 = std::stoi(m[1]);
                            int num2 = std::stoi(m[2]);
                            answer += num1 * num2;
                        }
                    } else {
                        std::cout << "No match at " << line.substr(0, 20) << std::endl;
                    }
                    line = line.substr(4); // Jump over "mul("
                }
            }
        }
        while (start_pos != std::string::npos);
    }

    std::cout << "Answer: " << answer << std::endl;
    // 181345830
}