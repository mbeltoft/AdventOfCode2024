#include <vector>
#include <string>

/// @brief Split string at delimeter and return a vector of strings with the result
/// @param s String to split
/// @param delimiter Delimiter (a std::string) to split string
/// @return Vector of strings
std::vector<std::string> StringSplit(const std::string &s, const std::string &delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }
    res.push_back(s.substr(pos_start));
    return res;
}

/// @brief Split string at delimeter and return a vector of strings with the result
/// @param s String to split
/// @param delimiter Delimiter (a single char) to split string
/// @return Vector of strings
std::vector<std::string> StringSplit(const std::string &s, char delimiter) {
    return StringSplit(s, std::string(1, delimiter));
}
