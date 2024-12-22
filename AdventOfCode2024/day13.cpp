#include <algorithm>
#include <cstdint>
#include <fstream>
#include <regex>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "day.hpp"

constexpr const int NUM_TOKENS_TO_PRESS_BUTTON_A = 3;
constexpr const int NUM_TOKENS_TO_PRESS_BUTTON_B = 1;
constexpr const int64_t PRIZE_COORDINATE_OFFSET = 10000000000000;

struct ClawMachine {
    int64_t button_a_x_distance;
    int64_t button_a_y_distance;
    int64_t button_b_x_distance;
    int64_t button_b_y_distance;
    int64_t prize_x_coordinate;
    int64_t prize_y_coordinate;
};

static auto parse_input(const bool with_prize_offset)
    -> std::vector<ClawMachine> {
    const std::string input_file_name{"data/day13.txt"};
    std::ifstream input_file{input_file_name};

    const std::regex retrieve_button_a_distances(
        R"(Button A: X\+(\d+), Y\+(\d+))");
    const std::regex retrieve_button_b_distances(
        R"(Button B: X\+(\d+), Y\+(\d+))");
    const std::regex retrieve_prize_coodinates(R"(Prize: X=(\d+), Y=(\d+))");

    std::vector<ClawMachine> claw_machines;
    std::string line;
    while(std::getline(input_file, line)) {
        if(line.empty()) {
            continue;
        }

        std::smatch m1;
        std::smatch m2;
        std::smatch m3;

        std::regex_match(line, m1, retrieve_button_a_distances);

        std::string line2;
        std::getline(input_file, line2);
        std::regex_match(line2, m2, retrieve_button_b_distances);

        std::string line3;
        std::getline(input_file, line3);
        std::regex_match(line3, m3, retrieve_prize_coodinates);

        claw_machines.push_back(
            ClawMachine{.button_a_x_distance = std::stoi(m1[1].str()),
                        .button_a_y_distance = std::stoi(m1[2].str()),
                        .button_b_x_distance = std::stoi(m2[1].str()),
                        .button_b_y_distance = std::stoi(m2[2].str()),
                        .prize_x_coordinate =
                            std::stoi(m3[1].str()) +
                            (with_prize_offset ? PRIZE_COORDINATE_OFFSET : 0),
                        .prize_y_coordinate =
                            std::stoi(m3[2].str()) +
                            (with_prize_offset ? PRIZE_COORDINATE_OFFSET : 0)});
    }

    return claw_machines;
}

auto find_min_tokens(const ClawMachine claw_machine) -> int64_t {
    const int64_t cannot_win_prize = 0;
    const int64_t determinant =
        claw_machine.button_a_x_distance * claw_machine.button_b_y_distance -
        claw_machine.button_a_y_distance * claw_machine.button_b_x_distance;
    if(determinant == 0) {
        throw std::invalid_argument(
            "The case for determinant 0 is not yet implemented.");
    }

    const int64_t num_times_to_press_a_before_dividing_by_determinant =
        claw_machine.button_b_y_distance * claw_machine.prize_x_coordinate -
        claw_machine.button_b_x_distance * claw_machine.prize_y_coordinate;
    const int64_t num_times_to_press_b_before_dividing_by_determinant =
        -claw_machine.button_a_y_distance * claw_machine.prize_x_coordinate +
        claw_machine.button_a_x_distance * claw_machine.prize_y_coordinate;

    if((num_times_to_press_a_before_dividing_by_determinant % determinant !=
        0) ||
       (num_times_to_press_b_before_dividing_by_determinant % determinant !=
        0)) {
        return cannot_win_prize;
    }

    const int64_t num_times_to_press_a =
        num_times_to_press_a_before_dividing_by_determinant / determinant;
    const int64_t num_times_to_press_b =
        num_times_to_press_b_before_dividing_by_determinant / determinant;
    const int64_t num_tokens =
        NUM_TOKENS_TO_PRESS_BUTTON_A * num_times_to_press_a +
        NUM_TOKENS_TO_PRESS_BUTTON_B * num_times_to_press_b;
    return num_tokens;
}

auto solve_day13a() -> int64_t {
    const std::vector<ClawMachine> claw_machines = parse_input(false);
    int64_t total_tokens_needed_to_win_all_prizes = 0;
    for(const ClawMachine claw_machine : claw_machines) {
        total_tokens_needed_to_win_all_prizes += find_min_tokens(claw_machine);
    }

    return total_tokens_needed_to_win_all_prizes;
}

auto solve_day13b() -> int64_t {
    const std::vector<ClawMachine> claw_machines = parse_input(true);
    int64_t total_tokens_needed_to_win_all_prizes = 0;
    for(const ClawMachine claw_machine : claw_machines) {
        total_tokens_needed_to_win_all_prizes += find_min_tokens(claw_machine);
    }

    return total_tokens_needed_to_win_all_prizes;
}