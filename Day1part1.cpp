#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

void main()
{
    std::ifstream file("input1.txt");
    std::string line;
    std::vector<int> list1;
    std::vector<int> list2;
    while (getline(file, line)) {
        int num1;
        int num2;
        std::stringstream ss;
        ss << line;
        ss >> num1 >> num2;
        list1.push_back(num1);
        list2.push_back(num2);
        std::cout << "Read " << num1 << " " << num2 << std::endl;
    }
    std::cout << "Sizes list1: " << list1.size() << ", list2: " << list2.size() << std::endl;

    std::sort(list1.begin(), list1.end(), std::greater<int>());
    std::sort(list2.begin(), list2.end(), std::greater<int>());

    int total_dist = 0;
    for (size_t i = 0; i < list1.size(); i++) {
        int num1 = list1[i];
        int num2 = list2[i];
        int dist = abs(num1 - num2);
        total_dist += dist;
        std::cout << num1 << " " << num2 << " dist: " << dist << std::endl;
    }
    std::cout << "Total dist: " << total_dist << std::endl;
    // 2815556
}
