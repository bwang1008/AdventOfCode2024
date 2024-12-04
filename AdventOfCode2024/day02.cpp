#include <cstdlib>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

#include "day.hpp"

constexpr const int INCREASE_LOWER_BOUND = 1;
constexpr const int INCREASE_UPPER_BOUND = 3;

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

auto parse_input() -> std::vector<std::vector<int>> {
    const std::string input_file_name{"data/day02.txt"};
    std::ifstream input_file{input_file_name};

    std::vector<std::vector<int>> input;
    std::string line;
    while(std::getline(input_file, line)) {
        const std::vector<std::string> tokens = split(line, ' ');
        std::vector<int> line_ints(tokens.size());
        for(std::size_t i = 0; i < tokens.size(); ++i) {
            const std::string token = tokens[i];
            line_ints[i] = std::stoi(token);
        }
        input.push_back(line_ints);
    }

    return input;
}

auto report_is_safe(const std::vector<int> &report) -> bool {
    if(report.size() < 2) {
        return true;
    }

    const bool initial_increasing = (report[0] < report[1]);
    for(std::size_t i = 1; i < report.size(); ++i) {
        const int diff = report[i] - report[i - 1];
        const bool currently_increasing = (diff > 0);

        if(!(initial_increasing == currently_increasing &&
             INCREASE_LOWER_BOUND <= std::abs(diff) &&
             std::abs(diff) <= INCREASE_UPPER_BOUND)) {
            return false;
        }
    }

    return true;
}

/**
 * Examples:
 * [10, 13, 9, 14, 15] - remove 9
 * [10, 13, 9, 8, 7] - remove 13
 * [7, 6, 7, 8] - remove first 7
 *
 */
auto report_is_safe_with_problem_dampener(std::vector<int> report) -> bool {
    if(report.size() < 2) {
        return true;
    }

    const bool initial_increasing = (report[1] > report[0]);
    for(std::size_t i = 0; i + 1 < report.size(); ++i) {
        const int diff = report[i + 1] - report[i];
        const bool currently_increasing = (diff > 0);

        if(!(initial_increasing == currently_increasing &&
             INCREASE_LOWER_BOUND <= std::abs(diff) &&
             std::abs(diff) <= INCREASE_UPPER_BOUND)) {

            auto pos_to_delete = report.begin();
            // try removing element i - 1
            if(i > 0) {
                const int level_i_1 = report[i - 1];
                std::advance(pos_to_delete, i - 1);
                report.erase(pos_to_delete);
                if(report_is_safe(report)) {
                    return true;
                }
                report.insert(pos_to_delete, level_i_1);
            }

            // try removing element i
            const int level_i = report[i];
            pos_to_delete = report.begin();
            std::advance(pos_to_delete, i);
            report.erase(pos_to_delete);

            if(report_is_safe(report)) {
                return true;
            }

            // try removing element i + 1
            report.insert(pos_to_delete, level_i);
            pos_to_delete = report.begin();
            std::advance(pos_to_delete, i + 1);
            report.erase(pos_to_delete);

            return report_is_safe(report);
        }
    }

    return true;
}

auto solve_day02a() -> int64_t {
    const std::vector<std::vector<int>> reports = parse_input();
    int64_t count = 0;
    for(const std::vector<int> &report : reports) {
        if(report_is_safe(report)) {
            ++count;
        }
    }

    return count;
}

auto solve_day02b() -> int64_t {
    const std::vector<std::vector<int>> reports = parse_input();
    int64_t count = 0;
    for(const std::vector<int> &report : reports) {
        if(report_is_safe_with_problem_dampener(report)) {
            ++count;
        }
    }

    return count;
}
