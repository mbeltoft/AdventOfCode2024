#include <fstream>
#include <iostream>
#include <vector>
#include <string>

void main()
{
    std::fstream file("input25.txt");
    std::string line;

    // Lock
    // #####
    // .####
    // .####
    // .####
    // .#.#.
    // .#...
    // .....
    // 05343

    // Key
    // .....
    // #....
    // #....
    // #...#
    // #.#.#
    // #.###
    // #####
    // 50213

    std::vector<std::vector<int>> locks;
    std::vector<std::vector<int>> keys;

    bool new_section = true;
    bool lock = false;
    bool key = false;
    int row = 0;
    int counters[5]{0};
    while (std::getline(file, line)) {
        if (line.size() == 0) {
            new_section = true;
            lock = false;
            key = false;
            row = 0;
            memset(counters, 0, sizeof(counters));
            continue;
        }
        // Last row?
        if (++row > 6) {
            // Don't count last row
            std::vector<int> heights;
            //std::cout << (key ? "Key " : "Lock ");
            for (int i = 0; i < 5; i++) {
                //std::cout << counters[i];
                heights.push_back(counters[i]);
            }
            //std::cout << std::endl;
            if (key) {
                keys.push_back(heights);
            }
            else if (lock) {
                locks.push_back(heights);
            }
            continue;
        }
        // Lock or Key?
        if (new_section) {
            new_section = false;
            if (line == "#####") {
                lock = true;
            }
            else if (line == ".....") {
                key = true;
            }
            else {
                std::cerr << "Panic" << std::endl;
                break;
            }
            continue;
        }
        for (size_t i = 0; i < line.size(); i++) {
            if (line[i] == '#') {
                counters[i]++;
            }
        }
    }
    std::cout << "Locks: " << locks.size() << std::endl;
    std::cout << "Keys: " << keys.size() << std::endl;

    int key_fits = 0;

    // Try every key in every lock
    for (const auto &lock : locks) {
        for (const auto &key : keys) {
            bool fail = false;
            for (size_t i = 0; i < key.size(); i++) {
                if (lock[i] + key[i] > 5) {
                    //std::cout << "Overlap at column " << i << std::endl;
                    fail = true;
                    break; // Next key
                }
            }
            if (!fail) {
                key_fits++;
            }
        }
    }
    std::cout << "Key and lock combinations that fit: " << key_fits << std::endl;
    // 3136
}