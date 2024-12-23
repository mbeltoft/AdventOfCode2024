// https://github.com/TeoInMaghine/AoC/blob/main/2024/Day%2017/2.cpp
#include <iostream>
#include <algorithm>
#include <cstdint>

uint64_t A = 0;

int main() {
    // I want it to print:
    int goals[16] = {2,4,1,2,7,5,0,3,4,7,1,7,5,5,3,0};
    std::reverse(goals, goals + 16);

    for (int goal : goals) {
        A <<= 3;

        int next_bits = 8;
        // from bigger to smaller to get the smallest possible "next_bits"
        for (int i = 7; i >= 0; i--) {
            uint64_t possible_A = A + i;
            int out = ((((possible_A & 7) ^ 2) ^ (possible_A >> ((possible_A & 7) ^ 2))) ^ 7) & 7;
            if (out == goal) next_bits = i;
        }

        A += next_bits;
    }

    std::cout << A << std::endl;
}