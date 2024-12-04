#include <fstream>
#include <regex>
#include <string>

#include "day.hpp"

static auto parse_input() -> std::string {
    const std::string input_file_name{"data/day03.txt"};
    std::ifstream input_file{input_file_name};

    std::string input;
    std::string line;
    while(std::getline(input_file, line)) {
        input += line;
    }

    return input;
}

auto solve_day03a() -> int64_t {
    // see https://en.cppreference.com/w/cpp/regex
    const std::regex mul_pattern(R"(mul\((\d+),(\d+)\))");
    int64_t sum = 0;

    const std::string memory = parse_input();
    auto mul_start =
        std::sregex_iterator(memory.begin(), memory.end(), mul_pattern);
    auto mul_end = std::sregex_iterator();

    for(std::sregex_iterator i = mul_start; i != mul_end; ++i) {
        std::smatch match = *i;
        const int left = std::stoi(match[1]);
        const int right = std::stoi(match[2]);
        sum += left * right;
    }
    return sum;
}

auto solve_day03b() -> int64_t {
    // see https://en.cppreference.com/w/cpp/regex
    const std::regex mul_pattern(R"(mul\((\d+),(\d+)\)|do\(\)|don't\(\))");
    int64_t sum = 0;
    bool enabled = true;

    const std::string memory = parse_input();
    auto mul_start =
        std::sregex_iterator(memory.begin(), memory.end(), mul_pattern);
    auto mul_end = std::sregex_iterator();

    for(std::sregex_iterator i = mul_start; i != mul_end; ++i) {
        std::smatch match = *i;

        if(match[0] == "do()") {
            enabled = true;
        } else if(match[0] == "don't()") {
            enabled = false;
        }
        if(match.str().substr(0, 3) == "mul" && enabled) {
            const int left = std::stoi(match[1]);
            const int right = std::stoi(match[2]);
            sum += left * right;
        }
    }
    return sum;
}
