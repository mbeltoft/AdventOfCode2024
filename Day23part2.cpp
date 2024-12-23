#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "StringSplit.h"

std::vector<std::string> computers;
std::vector<std::string> links;

void addEdge(std::vector<std::vector<int>> &mat, const std::string &ca, const std::string &cb)
{
    size_t a = 0;
    for (; a < computers.size(); a++) {
        if (ca == computers[a]) {
            break;
        }
    }
    size_t b = 0;
    for (; b < computers.size(); b++) {
        if (cb == computers[b]) {
            break;
        }
    }

    mat[a][b] = 1;
    mat[b][a] = 1; // Since the graph is undirected
}

void displayMatrix(std::vector<std::vector<int>> &mat)
{
    int V = mat.size();
    for (int i = 0; i < V; i++) {
        std::string comp = computers[i];
        std::cout << comp << " ";
        int sum = 0;
        for (int j = 0; j < V; j++) {
            std::cout << mat[i][j] << " ";
            sum += mat[i][j];
        }
        std::cout << "= " << sum << std::endl;
    }
}

void main()
{
    std::fstream file("input23test.txt");
    std::string line;

    // Create a graph with 4 vertices and no edges
    // Note that all values are initialized as 0
    int V = 16;
    std::vector<std::vector<int>> mat(V, std::vector<int>(V, 0));
    //std::vector<std::vector<int>> mat;

    // kh-tc
    while (std::getline(file, line)) {
        auto list = StringSplit(line, '-');
        for (const auto &computer : list) {
            if (find(computers.begin(), computers.end(), computer) == computers.end()) {
                computers.push_back(computer);
            }
        }
        addEdge(mat, list[0], list[1]);
        // Forward link
        links.push_back(line);
        // Backward link
        std::string link = list[1] + "-" + list[0];
        links.push_back(link);
    }
    std::cout << "Computers: " << computers.size() << std::endl;
    std::cout << "Links: " << links.size() << std::endl;

    displayMatrix(mat);
}