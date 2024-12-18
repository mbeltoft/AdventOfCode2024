#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <cstdint>

int64_t blink2(int64_t stone, int times, std::map<int64_t, std::map<int, int64_t>> &dictionary)
{
    if (times == 0) {
        return 1; // number of stones
    }

    auto d1 = dictionary.find(stone);
    if (d1 != dictionary.end()) {
        auto e = d1->second.find(times);
        if (e != d1->second.end()) {
            return e->second;
        }
    }

    std::string t = std::to_string(stone);
    int64_t e; // number of stones
    if (stone == 0) {
        e = blink2(1, times - 1, dictionary);
    }
    else if (t.size() % 2 == 0) {
        size_t half = t.size() / 2;
        int64_t left_stone = strtol(t.substr(0, half).c_str(), nullptr, 10);
        int64_t right_stone = strtol(t.substr(half).c_str(), nullptr, 10);
        e = blink2(left_stone, times - 1, dictionary) + blink2(right_stone, times - 1, dictionary);
    }
    else {
        e = blink2(stone * 2024, times - 1, dictionary);
    }

    if (d1 == dictionary.end()) {
        dictionary[stone].insert(std::make_pair(times, e));
        d1 = dictionary.find(stone);
    }
    d1->second.insert(std::make_pair(times, e));

    return e; // number of stones
}

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

    std::map<int64_t, std::map<int, int64_t>> dictionary;
    int64_t result = 0;
    for (long stone : stones) {
        result += blink2(stone, 75, dictionary);
    }

    std::cout << "Number of stones: " << result << std::endl;
    // 239321955280205
}
