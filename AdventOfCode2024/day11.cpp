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

auto blink_at_stone(const int64_t stone) -> std::vector<int64_t> {
    constexpr const int STONE_MULTIPLIER = 2024;
    if(stone == 0) {
        return std::vector<int64_t>{1};
    }

    const std::string stone_rep = std::to_string(stone);
    if(stone_rep.size() % 2 == 0) {
        const int64_t left_stone =
            std::stoll(stone_rep.substr(0, stone_rep.size() / 2));
        const int64_t right_stone =
            std::stoll(stone_rep.substr(stone_rep.size() / 2));
        return std::vector<int64_t>{left_stone, right_stone};
    }

    return std::vector<int64_t>{stone * STONE_MULTIPLIER};
}

auto blink_at_stones(const std::unordered_map<int64_t, int64_t> &initial_stones)
    -> std::unordered_map<int64_t, int64_t> {
    std::unordered_map<int64_t, int64_t> after;
    for(const auto stone_nums_and_counts : initial_stones) {
        const int64_t stone_value = stone_nums_and_counts.first;
        const int64_t stone_count = stone_nums_and_counts.second;
        const std::vector<int64_t> blinked_stone = blink_at_stone(stone_value);
        for(const int64_t post_stone : blinked_stone) {
            after[post_stone] += stone_count;
        }
    }

    return after;
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