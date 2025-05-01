#include <cmath>    // std::abs
#include <cstdint>  // std::size_t, int64_t
#include <fstream>  // std::ifstream
#include <iostream> // std::cout, std::endl
#include <map>
#include <string>  // std::getline
#include <utility> // std::pair
#include <vector>

#include "day.hpp"

namespace Day21 {

constexpr char ACCEPT_KEY = 'A';
constexpr char UNKNOWN_KEY = '?';

auto parse_input() -> std::vector<std::string> {
    const std::string input_file_name = "data/day21.txt";
    std::ifstream input_file{input_file_name};

    std::vector<std::string> passcodes;
    std::string line;

    while(std::getline(input_file, line)) {
        passcodes.push_back(line);
    }

    return passcodes;
}

auto navigate_keypad(const std::vector<std::string> &directional_keypad)
    -> std::map<std::pair<char, char>, std::string> {
    std::map<std::pair<char, char>, std::string>
        min_button_pushes_to_navigate_directional_keypad;

    std::map<char, std::pair<std::size_t, std::size_t>> button_positions;
    for(std::size_t i = 0; i < directional_keypad.size(); ++i) {
        for(std::size_t j = 0; j < directional_keypad[i].size(); ++j) {
            const char c = directional_keypad[i][j];
            button_positions[c] = std::pair<std::size_t, std::size_t>(i, j);
        }
    }

    for(auto it1 = button_positions.begin(); it1 != button_positions.end();
        ++it1) {
        for(auto it2 = button_positions.begin(); it2 != button_positions.end();
            ++it2) {
            const char start = it1->first;
            const char end = it2->first;
            if(start == UNKNOWN_KEY || end == UNKNOWN_KEY) {
                continue;
            }
            bool move_row_first = true;
            if(it1->second.second == button_positions[UNKNOWN_KEY].second) {
                move_row_first = false;
            }
            const int drow = static_cast<int>(it2->second.first) -
                             static_cast<int>(it1->second.first);
            const int dcol = static_cast<int>(it2->second.second) -
                             static_cast<int>(it1->second.second);
            std::vector<char> moves;
            const char row_move = (drow > 0) ? 'v' : '^';
            const char col_move = (dcol > 0) ? '>' : '<';
            if(move_row_first) {
                for(int i = 0; i < std::abs(drow); ++i) {
                    moves.push_back(row_move);
                }
            }
            for(int i = 0; i < std::abs(dcol); ++i) {
                moves.push_back(col_move);
            }
            if(!move_row_first) {
                for(int i = 0; i < std::abs(drow); ++i) {
                    moves.push_back(row_move);
                }
            }

            min_button_pushes_to_navigate_directional_keypad
                [std::pair<char, char>(start, end)] =
                    std::string(moves.begin(), moves.end());
        }
    }

    return min_button_pushes_to_navigate_directional_keypad;
}

auto print_keypad_directions(
    const std::map<std::pair<char, char>, std::string> &keypad_directions)
    -> void {
    for(const auto &keypad_direction : keypad_directions) {
        std::cout << "(" << keypad_direction.first.first << ", "
                  << keypad_direction.first.second << ") -> "
                  << keypad_direction.second << std::endl;
    }
}

auto step_one_layer_up(
    const std::string &initial_steps,
    const std::map<std::pair<char, char>, std::string> &keypad_directions)
    -> std::string {
    std::string steps;
    for(std::size_t i = 0; i + 1 < initial_steps.size(); ++i) {
        steps += keypad_directions.at(
            std::pair<char, char>(initial_steps[i], initial_steps[i + 1]));
        steps += std::string(1, ACCEPT_KEY);
    }

    return steps;
}

auto steps_to_enter_passcode(const std::string &passcode, const int num_robots)
    -> std::string {
    const std::vector<std::string> numeric_keypad{"789", "456", "123", "?0A"};
    const std::vector<std::string> directional_keypad{"?^A", "<v>"};
    const std::map<std::pair<char, char>, std::string>
        numeric_keypad_directions = Day21::navigate_keypad(numeric_keypad);
    const std::map<std::pair<char, char>, std::string>
        directional_keypad_directions =
            Day21::navigate_keypad(directional_keypad);

    print_keypad_directions(numeric_keypad_directions);
    std::cout << "====================================" << std::endl;
    print_keypad_directions(directional_keypad_directions);

    std::string steps = passcode;
    std::cout << "Start with passcode " << steps << std::endl;
    for(int i = 0; i < num_robots; ++i) {
        steps = step_one_layer_up(std::string(1, ACCEPT_KEY) + steps,
                                  (i == 0) ? numeric_keypad_directions
                                           : directional_keypad_directions);
        std::cout << "Now steps[" << steps.size() << "] = " << steps
                  << std::endl;
    }
    return steps;
}

} // namespace Day21

auto solve_day21a() -> int64_t {
    Day21::steps_to_enter_passcode("980A", 3);
    return 0;
}

auto solve_day21b() -> int64_t { return 0; }
