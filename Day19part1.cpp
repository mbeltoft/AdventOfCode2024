#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <utility>
#include <sstream>
#include <functional>

std::vector<std::pair<size_t, char>> dead_ends;

bool Match(const std::string& pattern, const std::map<char, std::vector<std::string>>& towels, size_t pos)
{
    //if (pos == 0) {
        //std::cout << "Pattern: " << pattern << ", pos: " << pos << ", len: " << pattern.size() << std::endl;
    //}
    if (pos >= pattern.size()) {
        return true;
    }

    // See if we've tried this combination of position and first letter before
    for (const auto &dead_end : dead_ends) {
        if (dead_end.first == pos && dead_end.second == pattern[pos]) {
            //std::cout << "Hit dead end with " << dead_end.second << " at pos " << dead_end.first << std::endl;
            return false;
        }
    }

    auto possible_towels = towels.find(pattern[pos]);
    if (possible_towels == towels.end()) {
        dead_ends.push_back(std::make_pair(pos, pattern[pos])); // Insert dead end
        return false;
    }
    //std::cout << "Found " << possible_towels->second.size() << " towels for " << pattern[pos] << std::endl;
    for (const auto& towel : possible_towels->second) {
        // Skip matching towels that are too long
        if (towel.size() > pattern.size() - pos) {
            continue;
        }
        if (pattern.find(towel, pos) == pos) {
            //std::cout << "Towel: " << towel << " at pos " << pos << std::endl;
            if (Match(pattern, towels, pos + towel.size())) {
                return true;
            }
        }
    }
    dead_ends.push_back(std::make_pair(pos, pattern[pos])); // Insert dead end
    return false;
}

void main()
{
    std::string line;
    int y = 0;
    bool read_patterns = false;
    std::vector<std::string> towels;
    std::vector<std::string> patterns;

    std::ifstream file("input19.txt");
    while (getline(file, line)) {
        if (line.size() == 0) {
            read_patterns = true;
        }
        if (read_patterns) {
            if (line.size()) {
                patterns.push_back(line);
            }
        } else {
            std::istringstream line_stream(line);
            std::string tmp;
            while (std::getline(line_stream, tmp, ',')) {
                // Trim leading whitespace
                while (tmp.size() > 0 && std::isspace(tmp[0])) {
                    tmp = tmp.substr(1);
                }
                towels.push_back(tmp);
            }
        }
    }
    std::cout << "Towels: " << towels.size() << std::endl;
    std::cout << "Patterns: " << patterns.size() << std::endl;

    // Create a map of towels for each starting character
    std::map<char, std::vector<std::string>> towels_map;
    for (const auto& towel : towels) {
        towels_map[towel[0]].push_back(towel);
    }
    std::cout << "Towel maps: " << towels_map.size() << std::endl;

    // Sort towels after length, so that longer towels are matched first
    //std::sort(towels.begin(), towels.end(), []
    //    (const std::string& first, const std::string& second){
    //        return first.size() > second.size();
    //    });

    int possible_designs = 0;
    for (const auto& pattern : patterns) {
        dead_ends.clear();
        //std::cout << "Pattern: " << ++count << "/" << patterns.size() << std::endl;
        if (Match(pattern, towels_map, 0)) {
            //std::cout << "Pattern '" << pattern << "' is possible" << std::endl;
            possible_designs++;
        } else {
            //std::cout << "Pattern '" << pattern << "' is not possible" << std::endl;
        }
    }
    std::cout << "Possible designs: " << possible_designs << std::endl;
    // 302
}
