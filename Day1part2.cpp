#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

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
        //std::cout << "Read " << num1 << " " << num2 << std::endl;
    }
    //std::cout << "Sizes list1: " << list1.size() << ", list2: " << list2.size() << std::endl;

    long total_similarity_score = 0;
    for (size_t i = 0; i < list1.size(); i++) {
        int num1 = list1[i];
        int count = 0;
        for (size_t j = 0; j < list2.size(); j++) {
            int num2 = list2[j];
            if (num1 == num2) {
                count++;
            }
        }
        int similarity_score = num1 * count;
        total_similarity_score += similarity_score;
    }
    std::cout << "Total similarity score: " << total_similarity_score << std::endl;
    // 23927637
}
