#include <cmath>    // std::abs
#include <cstdint>  // std::size_t, int64_t
#include <fstream>  // std::ifstream
#include <iostream> // std::cout, std::endl
#include <map>
#include <sstream> // std::stringstream
#include <string>  // std::getline, std::stoi
#include <utility> // std::pair
#include <vector>

#include "day.hpp"
#include "utils.hpp" // split()

namespace Day21 {

constexpr char ACCEPT_KEY = 'A';
constexpr char UNKNOWN_KEY = '?';

class PasscodeStepSummary {
  public:
    auto update_single(const std::string &substep, const int64_t count)
        -> void {
        this->substep_counts[substep] += count;
    }

    [[nodiscard]] auto get_single_count(const std::string &substep) const
        -> int64_t {
        return this->substep_counts.at(substep);
    }

    [[nodiscard]] auto get_all_substeps() const -> std::vector<std::string> {
        std::vector<std::string> result;
        for(const auto &substep_count : this->substep_counts) {
            if(substep_count.second > 0) {
                result.push_back(substep_count.first);
            }
        }

        return result;
    }

    [[nodiscard]] auto num_steps() const -> int64_t {
        int64_t total = 0;
        for(const auto &substep_count : this->substep_counts) {
            // plus one from implicit 'A' at the end of the substep.
            // Ex: we store {"<<v": 3} to mean we saw "<<vA" 3 times
            total += static_cast<int64_t>(substep_count.first.size() + 1) *
                     substep_count.second;
        }
        return total;
    }

    [[nodiscard]] auto to_string() const -> std::string {
        std::stringstream ss;
        ss << "PasscodeStepSummary(" << '\n';
        for(const auto &substep_count : this->substep_counts) {
            ss << "\t[" << substep_count.first << "] -> "
               << substep_count.second << "," << '\n';
        }
        ss << ")";
        return ss.str();
    }

  private:
    std::map<std::string, int64_t> substep_counts{};
};

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
            bool force_move_row_first =
                (std::pair<std::size_t, std::size_t>(it1->second.first,
                                                     it2->second.second) ==
                 button_positions[UNKNOWN_KEY]);
            bool force_move_col_first =
                (std::pair<std::size_t, std::size_t>(it2->second.first,
                                                     it1->second.second) ==
                 button_positions[UNKNOWN_KEY]);
            const int drow = static_cast<int>(it2->second.first) -
                             static_cast<int>(it1->second.first);
            const int dcol = static_cast<int>(it2->second.second) -
                             static_cast<int>(it1->second.second);
            std::vector<char> moves;
            const char row_move = (drow > 0) ? 'v' : '^';
            const char col_move = (dcol > 0) ? '>' : '<';

            /**
             * 0. upper left: going from 5 to 7
             * 1. ^<A   vs   <^A
             * 2. <Av<A>>^A    vs    v<<A>^A>A
             * 3.
             * v<<A >>^A v<A <A >>^A vAA <^A >A
             * vs
             * v<A <AA >>^A vA <^A >A vA ^A

             * Conclusion: prefer "<^A" over "^<A"

             * 0. upper right: going from 5 to 9
             * 1. ^>A   vs   >^A
             * 2. <Av>A^A   vs  vA<^A>A
             * 3.
             * v<<A >>^A v<A >A ^A <A >A
             * vs
             * v<A >^A v<<A >^A >A vA ^A

             * Conclusion: (^>A) == (>^A)
             * well first one has 17 patterns, second has 18.
             * So probably prefer (^>A) over (>^A)

             * 0. bottom left: going from 5 to 1
             * 1. v<A   vs   <vA
             * 2. v<A<A>>^A    vs    v<<A>A^>A
             * 3.
             * v<A <A >>^A v<<A >>^A vAA <^A >A
             * vs
             * v<A <AA >>^A vA ^A <A v>A ^A

             * Conclusion: prefer (<vA) to (v<A)

             * 0. bottom right: going from 5 to 3
             * 1. v>A   vs   >vA
             * 2. <vA>A^A   vs   vA<A>^A
             * 3.
             * v<<A >A >^A vA ^A <A >A
             * vs
             * <vA >^A v<<A >>^A vA <^A >A

             * Conclusion: prefer (v>A) over (>vA)
             */
            bool move_row_first = false;
            if(force_move_row_first) {
                move_row_first = true;
            }
            if(drow < 0 && dcol > 0) {
                // upper right
                move_row_first = true;
            }
            if(drow > 0 && dcol > 0) {
                // bottom right
                move_row_first = true;
            }
            move_row_first &= !force_move_col_first;
            // otherwise prefer moving by columns <> first

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

auto summarize_one_layer_up(
    const PasscodeStepSummary &initial_summary,
    const std::map<std::pair<char, char>, std::string> &keypad_directions)
    -> PasscodeStepSummary {
    PasscodeStepSummary layer_up_summary;
    for(const std::string &substep : initial_summary.get_all_substeps()) {
        const int64_t substep_initial_count =
            initial_summary.get_single_count(substep);
        const std::string substep_layer_up =
            step_one_layer_up(std::string(1, ACCEPT_KEY)
                                  .append(substep)
                                  .append(std::string(1, ACCEPT_KEY)),
                              keypad_directions);

        // std::cout << "substep " << substep << " -> " << substep_layer_up <<
        // std::endl;
        const std::vector<std::string> substep_layer_up_substeps =
            split(substep_layer_up, ACCEPT_KEY);
        // split("vAvA", 'A') returns ["v", "v", ""].
        // We only want to count "" for instances like "AA", not because of the
        // last split.
        for(std::size_t i = 0; i + 1 < substep_layer_up_substeps.size(); ++i) {
            const std::string substep_layer_up_substep =
                substep_layer_up_substeps[i];
            layer_up_summary.update_single(substep_layer_up_substep,
                                           substep_initial_count);
        }
    }

    return layer_up_summary;
}

auto step_summaries_to_enter_passcodes(
    const std::vector<std::string> &passcodes, const int num_robots)
    -> std::vector<PasscodeStepSummary> {
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

    std::vector<PasscodeStepSummary> step_summary_for_each_passcode;

    for(const std::string &passcode : passcodes) {
        PasscodeStepSummary summary;
        summary.update_single(passcode.substr(0, passcode.size() - 1), 1);
        std::cout << "Start with passcode " << passcode << std::endl;
        for(int i = 0; i < num_robots; ++i) {
            // each robot starts at button A.
            // robot closest to Historian has numeric keypad; everyone else has
            // directional keypad
            summary = summarize_one_layer_up(
                summary, (i == 0) ? numeric_keypad_directions
                                  : directional_keypad_directions);
            // std::cout << "Robot[" << 1 + i << "]: " << std::endl;
            // std::cout << summary.to_string() << std::endl;
            // std::cout << "with num_chars = " << summary.num_steps() <<
            // std::endl;
        }

        step_summary_for_each_passcode.push_back(summary);
    }

    return step_summary_for_each_passcode;
}

auto compute_total_complexity(const std::vector<std::string> &passcodes,
                              const std::vector<PasscodeStepSummary> &summaries)
    -> int64_t {
    int64_t total_complexity = 0;
    for(std::size_t i = 0; i < passcodes.size(); ++i) {
        const std::string passcode = passcodes[i];
        total_complexity += summaries[i].num_steps() *
                            std::stoi(passcode.substr(0, passcode.size() - 1));
    }

    return total_complexity;
}

} // namespace Day21

auto solve_day21a() -> int64_t {
    const std::vector<std::string> passcodes = Day21::parse_input();
    const int num_robots = 3;
    const std::vector<Day21::PasscodeStepSummary> passcodes_summaries =
        Day21::step_summaries_to_enter_passcodes(passcodes, num_robots);
    return Day21::compute_total_complexity(passcodes, passcodes_summaries);
}

auto solve_day21b() -> int64_t {
    const std::vector<std::string> passcodes = Day21::parse_input();
    const int num_robots = 26;
    const std::vector<Day21::PasscodeStepSummary> passcodes_summaries =
        Day21::step_summaries_to_enter_passcodes(passcodes, num_robots);
    return Day21::compute_total_complexity(passcodes, passcodes_summaries);
}

/**
Start with passcode 029A
Now steps[12] = <A^A^^>AvvvA
Now steps[28] = v<<A>>^A<A>A<AAv>A^A<vAAA^>A
Now steps[68] =
<vA<AA>>^AvAA<^A>Av<<A>>^AvA^Av<<A>>^AA<vA>A^A<A>Av<<A>A^>AAA<Av>A^A
*/