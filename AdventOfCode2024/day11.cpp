#include <algorithm>
#include <cstdint>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "day.hpp"
#include "utils.hpp"

static auto parse_input() -> std::vector<int64_t> {
    const std::string input_file_name{"data/day11.txt"};
    std::ifstream input_file{input_file_name};

    std::string line;
    std::getline(input_file, line);
    const std::vector<std::string> parts = split(line, ' ');

    std::vector<int64_t> stone_values(parts.size());
    std::transform(
        parts.cbegin(), parts.cend(), stone_values.begin(),
        [](const std::string &s) -> int64_t { return std::stoll(s); });

    return stone_values;
}

auto blink_at_stones(const std::unordered_map<int64_t, int64_t> &initial_stones)
    -> std::unordered_map<int64_t, int64_t> {
    constexpr const int STONE_MULTIPLIER = 2024;
    std::unordered_map<int64_t, int64_t> next_stones;

    for(const auto prior_value_and_count : initial_stones) {
        const int64_t value = prior_value_and_count.first;
        const int64_t count = prior_value_and_count.second;
        if(value == 0) {
            next_stones[1] += count;
            continue;
        }

        const std::string stone_rep = std::to_string(value);
        if(stone_rep.size() % 2 == 0) {
            const int64_t left_stone =
                std::stoll(stone_rep.substr(0, stone_rep.size() / 2));
            const int64_t right_stone =
                std::stoll(stone_rep.substr(stone_rep.size() / 2));
            next_stones[left_stone] += count;
            next_stones[right_stone] += count;
        } else {
            next_stones[value * STONE_MULTIPLIER] += count;
        }
    }

    return next_stones;
}

auto blink_at_stones_many_times(const std::vector<int64_t> &stones,
                                const int num_times_to_blink) -> int64_t {
    std::unordered_map<int64_t, int64_t> stone_value_to_count;
    for(const int64_t stone : stones) {
        stone_value_to_count[stone]++;
    }

    for(int i = 0; i < num_times_to_blink; ++i) {
        stone_value_to_count = blink_at_stones(stone_value_to_count);
    }

    int64_t total_number_of_stones = 0;
    for(const auto stone_value_and_counts : stone_value_to_count) {
        total_number_of_stones += stone_value_and_counts.second;
    }
    return total_number_of_stones;
}

auto solve_day11a() -> int64_t {
    constexpr const int NUM_TIMES_TO_BLINK = 25;
    const std::vector<int64_t> stones = parse_input();
    return blink_at_stones_many_times(stones, NUM_TIMES_TO_BLINK);
}

auto solve_day11b() -> int64_t {
    constexpr const int NUM_TIMES_TO_BLINK = 75;
    const std::vector<int64_t> stones = parse_input();
    return blink_at_stones_many_times(stones, NUM_TIMES_TO_BLINK);
}