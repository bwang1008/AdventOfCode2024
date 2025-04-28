#include <cstdint> // std::size_t, int64_t
#include <fstream> // std::ifstream
#include <set>
#include <string>  // std::getline
#include <utility> // std::pair
#include <vector>

#include "day.hpp"

namespace Day19 {

auto split(const std::string &text, const std::string &pattern)
    -> std::vector<std::string> {
    std::size_t prev = 0;
    std::size_t curr = 0;
    std::vector<std::string> output;
    while((curr = text.find(pattern, prev)) != std::string::npos) {
        output.push_back(text.substr(prev, curr - prev));
        prev = curr + pattern.size();
    }
    output.push_back(text.substr(prev, text.size() - prev));
    return output;
}

auto parse_input()
    -> std::pair<std::vector<std::string>, std::vector<std::string>> {
    const std::string input_file_name = "data/day19.txt";
    std::ifstream input_file{input_file_name};

    std::string patterns_line;
    std::getline(input_file, patterns_line);
    const std::vector<std::string> patterns = split(patterns_line, ", ");

    std::vector<std::string> desired;
    std::string line;
    while(std::getline(input_file, line)) {
        if(!line.empty()) {
            desired.push_back(line);
        }
    }

    return std::pair<std::vector<std::string>, std::vector<std::string>>(
        patterns, desired);
}

auto build_output_from_patterns(const std::string &output,
                                const std::set<std::string> &patterns) -> bool {
    std::vector<std::vector<bool>> dp = std::vector<std::vector<bool>>(
        output.size(), std::vector<bool>(1 + output.size(), false));

    for(std::size_t size = 1; size <= output.size(); ++size) {
        for(std::size_t start = 0; start + size <= output.size(); ++start) {
            const std::size_t end = start + size;
            const std::string sub = output.substr(start, size);
            if(patterns.find(sub) != patterns.end()) {
                dp[start][end] = true;
            } else {
                for(std::size_t mid = 1 + start; mid < end; ++mid) {
                    if(dp[start][mid] && dp[mid][end]) {
                        dp[start][end] = true;
                        break;
                    }
                }
            }
        }
    }

    return dp[0][output.size()];
}

} // namespace Day19

auto solve_day19a() -> int64_t {
    const std::pair<std::vector<std::string>, std::vector<std::string>> inputs =
        Day19::parse_input();
    const std::vector<std::string> patterns = inputs.first;
    const std::vector<std::string> desired = inputs.second;
    const std::set<std::string> patterns_set(patterns.begin(), patterns.end());
    int64_t count = 0;
    for(const std::string &desired1 : desired) {
        if(Day19::build_output_from_patterns(desired1, patterns_set)) {
            ++count;
        }
    }

    return count;
}

auto solve_day19b() -> int64_t { return 0; }