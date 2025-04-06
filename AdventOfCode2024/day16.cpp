#include <algorithm> // std::min
#include <cstdint>   // std::size_t, int64_t
#include <fstream>   // std::ifstream
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <stdexcept> // std::runtime_error
#include <string>
#include <utility> // std::pair
#include <vector>

#include "day.hpp"

namespace Day16 {

constexpr char START_SYMBOL = 'S';
constexpr char END_SYMBOL = 'E';
constexpr char WALL_SYMBOL = '#';
constexpr char EMPTY_SYMBOL = '.';
constexpr int64_t NUMBER_OF_DIFFERENT_ROTATIONS = 4;
constexpr int64_t SCORE_PER_ROTATION = 1000;

static auto parse_input() -> std::vector<std::vector<char>> {
    const std::string input_file_name{"data/day16.txt"};
    std::ifstream input_file{input_file_name};

    std::vector<std::vector<char>> board;
    std::string line;

    while(std::getline(input_file, line)) {
        board.emplace_back(line.begin(), line.end());
    }

    return board;
}

auto is_perpendicular(const std::pair<int, int> direction1,
                      const std::pair<int, int> direction2) -> bool {
    return (direction1.first * direction2.first +
            direction1.second * direction2.second) == 0;
}

class State {
  public:
    State(const std::size_t row_, const std::size_t col_,
          const std::pair<int, int> direction_)
        : row(row_), col(col_), direction(direction_) {}

    [[nodiscard]] auto get_row() const -> std::size_t { return row; }

    [[nodiscard]] auto get_col() const -> std::size_t { return col; }

    [[nodiscard]] auto get_direction() const -> std::pair<int, int> {
        return direction;
    }

    [[nodiscard]] auto to_string() const -> std::string {
        return "(" + std::to_string(row) + ", " + std::to_string(col) + ", <" +
               std::to_string(direction.first) + ", " +
               std::to_string(direction.second) + ">)";
    }

    auto operator<(const State &other) const -> bool {
        if(row != other.row) {
            return row < other.row;
        }
        if(col != other.col) {
            return col < other.col;
        }
        if(direction.first != other.direction.first) {
            return direction.first < other.direction.first;
        }
        return direction.second < other.direction.second;
        ;
    }

    [[nodiscard]] auto
    get_potential_neighbors(const std::size_t num_board_rows,
                            const std::size_t num_board_cols) const
        -> std::vector<std::pair<State, int64_t>> {
        std::vector<std::pair<State, int64_t>> potential_neighbors;
        if(row > 0 && direction == std::pair<int, int>(-1, 0)) {
            potential_neighbors.emplace_back(State(row - 1, col, direction), 1);
        }
        if(row + 1 < num_board_rows && direction == std::pair<int, int>(1, 0)) {
            potential_neighbors.emplace_back(State(row + 1, col, direction), 1);
        }
        if(col > 0 && direction == std::pair<int, int>(0, -1)) {
            potential_neighbors.emplace_back(State(row, col - 1, direction), 1);
        }
        if(col + 1 < num_board_cols && direction == std::pair<int, int>(0, 1)) {
            potential_neighbors.emplace_back(State(row, col + 1, direction), 1);
        }
        const std::vector<std::pair<int, int>> all_directions = {
            {0, 1}, {1, 0}, {0, -1}, {-1, 0}};
        for(const std::pair<int, int> potential_direction : all_directions) {
            if(is_perpendicular(potential_direction, direction)) {
                potential_neighbors.emplace_back(
                    State(row, col, potential_direction), SCORE_PER_ROTATION);
            }
        }

        return potential_neighbors;
    }

  private:
    std::size_t row;
    std::size_t col;
    std::pair<int, int> direction;
};

auto find_symbol(const std::vector<std::vector<char>> &board, const char symbol)
    -> std::pair<std::size_t, std::size_t> {
    for(std::size_t i = 0; i < board.size(); ++i) {
        for(std::size_t j = 0; j < board[i].size(); ++j) {
            if(board[i][j] == symbol) {
                return std::pair<std::size_t, std::size_t>(i, j);
            }
        }
    }

    const std::string error_message =
        "Did not find " + std::string(1, symbol) + " in board";
    throw std::invalid_argument(error_message);
}

auto compute_impossible_score(const std::size_t num_board_rows,
                              const std::size_t num_board_cols) -> int64_t {
    return 1 + (NUMBER_OF_DIFFERENT_ROTATIONS * SCORE_PER_ROTATION + 1) *
                   static_cast<int64_t>(num_board_rows) *
                   static_cast<int64_t>(num_board_cols);
}

auto find_min_score_of_all_positions(
    const std::vector<std::vector<char>> &board) -> std::map<State, int64_t> {
    const int64_t impossible_score =
        compute_impossible_score(board.size(), board[0].size());

    const std::pair<std::size_t, std::size_t> starting_position =
        find_symbol(board, START_SYMBOL);
    const State initial_state(starting_position.first, starting_position.second,
                              std::pair<int, int>(0, 1));
    std::map<State, int64_t> score_board;
    score_board[initial_state] = 0;

    std::set<State> visited;
    std::list<State> states_to_consider;
    states_to_consider.push_back(initial_state);

    while(!states_to_consider.empty()) {
        // get state with min score
        auto min_cost_iterator = states_to_consider.begin();
        int64_t min_cost_so_far = impossible_score;
        if(score_board.find(*min_cost_iterator) != score_board.end()) {
            min_cost_so_far = score_board[*min_cost_iterator];
        }
        for(auto it = states_to_consider.begin();
            it != states_to_consider.end(); ++it) {
            if(score_board.find(*it) != score_board.end()) {
                const int64_t cost_to_consider = score_board[*it];
                if(cost_to_consider < min_cost_so_far) {
                    min_cost_iterator = it;
                    min_cost_so_far = cost_to_consider;
                }
            }
        }

        const State current_state = *min_cost_iterator;
        states_to_consider.erase(min_cost_iterator);

        if(visited.find(current_state) != visited.end()) {
            continue;
        }
        visited.insert(current_state);

        const std::vector<std::pair<State, int64_t>> potential_neighbors =
            current_state.get_potential_neighbors(board.size(),
                                                  board[0].size());
        for(const std::pair<State, int64_t> potential_next_state_and_cost :
            potential_neighbors) {
            const State potential_neighbor =
                potential_next_state_and_cost.first;
            const int64_t cost_of_moving_to_this_next_state =
                potential_next_state_and_cost.second;
            if(score_board.find(potential_neighbor) == score_board.end()) {
                score_board[potential_neighbor] = impossible_score;
            }

            const char existing_symbol = board[potential_neighbor.get_row()]
                                              [potential_neighbor.get_col()];
            if(existing_symbol != WALL_SYMBOL &&
               visited.find(potential_neighbor) == visited.end()) {
                score_board[potential_neighbor] =
                    std::min(score_board[current_state] +
                                 cost_of_moving_to_this_next_state,
                             score_board[potential_neighbor]);
                states_to_consider.push_back(potential_neighbor);
            }
        }
    }

    return score_board;
}

auto find_min_score_of_end_state(const std::vector<std::vector<char>> &board)
    -> int64_t {
    const int64_t impossible_score =
        compute_impossible_score(board.size(), board[0].size());
    const std::map<State, int64_t> score_board =
        find_min_score_of_all_positions(board);
    const std::pair<std::size_t, std::size_t> end_position =
        find_symbol(board, END_SYMBOL);
    const State final_state1 = State(end_position.first, end_position.second,
                                     std::pair<int, int>(0, 1));
    const State final_state2 = State(end_position.first, end_position.second,
                                     std::pair<int, int>(1, 0));
    const State final_state3 = State(end_position.first, end_position.second,
                                     std::pair<int, int>(0, -1));
    const State final_state4 = State(end_position.first, end_position.second,
                                     std::pair<int, int>(-1, 0));
    const std::vector<State> possible_final_states{final_state1, final_state2,
                                                   final_state3, final_state4};

    int64_t end_score = impossible_score;
    for(const State possible_final_state : possible_final_states) {
        if(score_board.find(possible_final_state) != score_board.end()) {
            const int64_t possible_final_score =
                score_board.at(possible_final_state);
            end_score = std::min(possible_final_score, end_score);
        }
    }

    return end_score;
}

} // namespace Day16

auto solve_day16a() -> int64_t {
    const std::vector<std::vector<char>> board = Day16::parse_input();
    return Day16::find_min_score_of_end_state(board);
}

auto solve_day16b() -> int64_t { return 0; }
