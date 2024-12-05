#include <cstddef>
#include <string>
#include <vector>

#include "utils.hpp"

auto split(const std::string &text, const char delimiter)
    -> std::vector<std::string> {
    std::vector<std::string> tokens;
    std::size_t last = 0;
    std::size_t next = 0;
    while((next = text.find(delimiter, last)) != std::string::npos) {
        tokens.push_back(text.substr(last, next - last));
        last = next + 1;
    }
    tokens.push_back(text.substr(last));

    return tokens;
}
