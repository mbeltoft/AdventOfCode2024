#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "StringSplit.h"

std::vector<std::string> computers;
std::vector<std::string> links;

void main()
{
    std::fstream file("input23.txt");
    std::string line;

    // kh-tc
    while (std::getline(file, line)) {
        auto list = StringSplit(line, '-');
        for (const auto &computer : list) {
            if (find(computers.begin(), computers.end(), computer) == computers.end()) {
                computers.push_back(computer);
            }
        }
        // Forward link
        links.push_back(line);
        // Backward link
        std::string link = list[1] + "-" + list[0];
        links.push_back(link);
    }
    std::cout << "Computers: " << computers.size() << std::endl;
    std::cout << "Links: " << links.size() << std::endl;

    int t_count = 0;
    for (size_t a = 0; a < computers.size(); a++) {
        std::string ca = computers[a];
        for (size_t b = a; b < computers.size(); b++) {
            std::string cb = computers[b];
            // Are computers a and b linked?
            std::string forward_link = ca + "-" + cb;
            std::string backward_link = cb + "-" + ca;
            if (std::find(links.begin(), links.end(), forward_link) != links.end() ||
                std::find(links.begin(), links.end(), backward_link) != links.end()) {
                for (size_t c = b; c < computers.size(); c++) {
                    std::string cc = computers[c];
                    // Are computers a and c linked?
                    std::string forward_link = ca + "-" + cc;
                    std::string backward_link = cc + "-" + ca;
                    if (std::find(links.begin(), links.end(), forward_link) != links.end() ||
                        std::find(links.begin(), links.end(), backward_link) != links.end()) {
                        // Are computers b and c linked?
                        std::string forward_link = cb + "-" + cc;
                        std::string backward_link = cc + "-" + cb;
                        if (std::find(links.begin(), links.end(), forward_link) != links.end() ||
                            std::find(links.begin(), links.end(), backward_link) != links.end()) {
                            //std::cout << ca << "-" << cb << "-" << cc << std::endl;
                            if (ca[0] == 't' || cb[0] == 't' || cc[0] == 't') {
                                t_count++;
                            }
                        }
                    }
                }
            }
        }
    }
    std::cout << "Links containing a t-computer: " << t_count << std::endl;
    // 1154
}