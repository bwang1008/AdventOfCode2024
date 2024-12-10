#include <algorithm>
#include <cstddef>
#include <fstream>
#include <functional> // std::function, std::multiplies, std::plus
#include <string>     // std::stoll, std::string, std::to_string
#include <utility>
#include <vector>

#include "day.hpp"
#include "utils.hpp"

static auto parse_input()
    -> std::vector<std::pair<int64_t, std::vector<int64_t>>> {
    const std::string input_file_name{"data/day07.txt"};
    std::ifstream input_file{input_file_name};

    std::vector<std::pair<int64_t, std::vector<int64_t>>> input;
    std::string line;
    while(std::getline(input_file, line)) {
        const std::vector<std::string> total_and_parts = split(line, ':');
        const int64_t total = std::stoll(total_and_parts[0]);
        const std::vector<std::string> parts =
            split(total_and_parts.back().substr(1), ' ');
        std::vector<int64_t> numeric_parts(parts.size());
        std::transform(
            parts.cbegin(), parts.cend(), numeric_parts.begin(),
            [](const std::string &s) -> int64_t { return std::stoll(s); });

        input.emplace_back(total, numeric_parts);
    }

    return input;
}

auto match_equation_helper(
    const std::vector<int64_t> &numbers,
    const std::vector<std::function<int64_t(int64_t, int64_t)>> &operators,
    const int64_t desired_total, const std::size_t current_number_index,
    const int64_t current_total) -> bool {
    if(1 + current_number_index == numbers.size()) {
        return current_total == desired_total;
    }

    for(const std::function<int64_t(int64_t, int64_t)> &binary_operator :
        operators) {
        if(match_equation_helper(
               numbers, operators, desired_total, 1 + current_number_index,
               binary_operator(current_total,
                               numbers[1 + current_number_index]))) {
            return true;
        }
    }

    return false;
}

auto match_equation(
    const std::vector<int64_t> &numbers, const int64_t desired_total,
    const std::vector<std::function<int64_t(int64_t, int64_t)>> &operators)
    -> bool {
    return match_equation_helper(numbers, operators, desired_total, 0,
                                 numbers.front());
}

auto solve_day07a() -> int64_t {
    const std::vector<std::pair<int64_t, std::vector<int64_t>>> equations =
        parse_input();
    const std::vector<std::function<int64_t(int64_t, int64_t)>> operators{
        std::plus<>(), std::multiplies<>()};

    int64_t total = 0;
    for(const std::pair<int64_t, std::vector<int64_t>> &total_and_equation :
        equations) {
        if(match_equation(total_and_equation.second, total_and_equation.first,
                          operators)) {
            total += total_and_equation.first;
        }
    }

    return total;
}

auto solve_day07b() -> int64_t {
    const std::vector<std::pair<int64_t, std::vector<int64_t>>> equations =
        parse_input();
    const std::vector<std::function<int64_t(int64_t, int64_t)>> operators{
        std::plus<>(), std::multiplies<>(),
        [](const int64_t a, const int64_t b) -> int64_t {
            return std::stoll(std::to_string(a) + std::to_string(b));
        }};

    int64_t total = 0;
    for(const std::pair<int64_t, std::vector<int64_t>> &total_and_equation :
        equations) {
        if(match_equation(total_and_equation.second, total_and_equation.first,
                          operators)) {
            total += total_and_equation.first;
        }
    }

    return total;
}
