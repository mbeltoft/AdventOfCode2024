#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdint>

void main()
{
    std::fstream file("input11.txt");
    std::string line;
    std::vector<int64_t> stones;

    // Load map and find trailheads
    while (std::getline(file, line)) {
        std::string tmp;
        std::stringstream ss(line);

        while(getline(ss, tmp, ' ')) {
            stones.push_back(atoi(tmp.c_str()));
        }
    }

    for (int i = 0; i < 25; i++) {
        std::vector<int64_t> new_stones;
        for (int64_t stone : stones) {
            std::string number_str = std::to_string(stone);
            // Rule 1
            if (stone == 0) {
                new_stones.push_back(1);
            }
            // Rule 2
            else if (number_str.size() % 2 == 0) {
                size_t half = number_str.size() / 2;
                std::string first_half = number_str.substr(0, half);
                std::string second_half = number_str.substr(half);
                new_stones.push_back(strtol(first_half.c_str(), nullptr, 10));
                new_stones.push_back(strtol(second_half.c_str(), nullptr, 10));
            }
            // Rule 3
            else {
                new_stones.push_back(2024 * stone);
            }
        }
        stones.swap(new_stones);
        std::cout << "Iteration " << i+1 << ", stones: " << stones.size() << std::endl;
    }

    //for (auto stone : stones) {
    //    std::cout << stone << " ";
    //}
    //std::cout << std::endl;
    std::cout << "Number of stones: " << stones.size() << std::endl;
    // 202019
}
