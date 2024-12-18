#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

void main()
{
    int report_number = 0;
    int safe_reports = 0;
    std::ifstream file("input2.txt");
    std::string line;
    while (getline(file, line)) {
        report_number++;
        std::vector<int> report;
        std::stringstream ss;
        ss << line;
        int num = 0;
        while (ss >> num) {
            report.push_back(num);
        }
        //std::cout << "Report size: " << report.size() << std::endl;

        int old_num = -1000;
        int direction = 0;
        bool safe = true;
        for (auto num : report) {
            //std::cout << num << std::endl;
            if (old_num == -1000) {
                // First number in report
                old_num = num;
            } else {
                // Subsequent numbers in report
                if (num == old_num) {
                    //std::cout << "Flatliner" << std::endl;
                    safe = false;
                    break;
                }
                int diff = num - old_num;
                if (abs(diff) > 3) {
                    //std::cout << "Jump" << std::endl;
                    safe = false;
                    break;
                }
                int new_direction = 0;
                if (diff > 0) {
                    new_direction = 1;
                } else if (diff < 0) {
                    new_direction = -1;
                }
                if (direction == 0) {
                    direction = new_direction;
                } else {
                    if (new_direction != direction) {
                        //std::cout << "Direction change" << std::endl;
                        safe = false;
                        break;
                    }
                }
            }
            old_num = num;
        }
        if (safe) {
            safe_reports++;
            std::cout << "Report: " << report_number << " : ";
            for (auto num : report) {
                std::cout << num << " ";
            }
            std::cout << "is safe" << std::endl;
        }
    }
    std::cout << "Safe reports: " << safe_reports << std::endl;
    // 230
}
