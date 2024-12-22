#include <algorithm>
#include <cstdint>
#include <fstream>
#include <regex>
#include <string>
#include <utility>
#include <vector>

#include "day.hpp"

constexpr const int MAX_NUMBER_OF_BUTTON_PRESSES = 100;
constexpr const int NUM_TOKENS_TO_PRESS_BUTTON_A = 3;
constexpr const int NUM_TOKENS_TO_PRESS_BUTTON_B = 1;

struct ClawMachine {
    int button_a_x_distance;
    int button_a_y_distance;
    int button_b_x_distance;
    int button_b_y_distance;
    int prize_x_coordinate;
    int prize_y_coordinate;
};

static auto parse_input() -> std::vector<ClawMachine> {
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
                        .prize_x_coordinate = std::stoi(m3[1].str()),
                        .prize_y_coordinate = std::stoi(m3[2].str())});
    }

    return claw_machines;
}

auto find_min_tokens(const ClawMachine claw_machine) -> std::pair<bool, int> {
    const std::pair<bool, int> cannot_win_prize(false, 0);
    const int determinant =
        claw_machine.button_a_x_distance * claw_machine.button_b_y_distance -
        claw_machine.button_a_y_distance * claw_machine.button_b_x_distance;
    if(determinant == 0) {
        const int max_amount_of_tokens =
            NUM_TOKENS_TO_PRESS_BUTTON_A * MAX_NUMBER_OF_BUTTON_PRESSES +
            NUM_TOKENS_TO_PRESS_BUTTON_B * MAX_NUMBER_OF_BUTTON_PRESSES + 1;
        int min_amount_of_tokens = max_amount_of_tokens;
        for(int num_press_a = 0; num_press_a < MAX_NUMBER_OF_BUTTON_PRESSES;
            ++num_press_a) {
            const int num_press_b =
                (claw_machine.prize_x_coordinate -
                 num_press_a * claw_machine.button_a_x_distance) /
                claw_machine.button_b_x_distance;
            if(num_press_b >= 0 &&
               (num_press_a * claw_machine.button_a_x_distance +
                    num_press_b * claw_machine.button_b_x_distance ==
                claw_machine.prize_x_coordinate) &&
               (num_press_a * claw_machine.button_a_y_distance +
                    num_press_b * claw_machine.button_b_y_distance ==
                claw_machine.prize_y_coordinate)) {
                const int num_tokens =
                    NUM_TOKENS_TO_PRESS_BUTTON_A * num_press_a +
                    NUM_TOKENS_TO_PRESS_BUTTON_B * num_press_b;
                min_amount_of_tokens =
                    std::min(num_tokens, min_amount_of_tokens);
            }
        }

        if(min_amount_of_tokens == max_amount_of_tokens) {
            return cannot_win_prize;
        }
        return std::pair(true, min_amount_of_tokens);
    }

    const int num_times_to_press_a_before_dividing_by_determinant =
        claw_machine.button_b_y_distance * claw_machine.prize_x_coordinate -
        claw_machine.button_b_x_distance * claw_machine.prize_y_coordinate;
    const int num_times_to_press_b_before_dividing_by_determinant =
        -claw_machine.button_a_y_distance * claw_machine.prize_x_coordinate +
        claw_machine.button_a_x_distance * claw_machine.prize_y_coordinate;

    if((num_times_to_press_a_before_dividing_by_determinant % determinant !=
        0) ||
       (num_times_to_press_b_before_dividing_by_determinant % determinant !=
        0)) {
        return cannot_win_prize;
    }

    const int num_times_to_press_a =
        num_times_to_press_a_before_dividing_by_determinant / determinant;
    const int num_times_to_press_b =
        num_times_to_press_b_before_dividing_by_determinant / determinant;
    const int num_tokens = NUM_TOKENS_TO_PRESS_BUTTON_A * num_times_to_press_a +
                           NUM_TOKENS_TO_PRESS_BUTTON_B * num_times_to_press_b;
    return std::pair(true, num_tokens);
}

auto solve_day13a() -> int64_t {
    const std::vector<ClawMachine> claw_machines = parse_input();
    int total_tokens_needed_to_win_all_prizes = 0;
    for(const ClawMachine claw_machine : claw_machines) {
        const std::pair<bool, int> results_of_claw_machine =
            find_min_tokens(claw_machine);
        if(results_of_claw_machine.first) {
            total_tokens_needed_to_win_all_prizes +=
                results_of_claw_machine.second;
        }
    }

    return total_tokens_needed_to_win_all_prizes;
}

auto solve_day13b() -> int64_t {
    const int result = 0;
    return result;
}