#include <cstdint> // std::size_t
#include <fstream> // std::ifstream
#include <iostream>
#include <map>
#include <set>
#include <stdexcept> // std::invalid_argument, std::runtime_error
#include <string>
#include <utility> // std::pair, std::move
#include <vector>

#include "day.hpp"

namespace Day15 {

constexpr char BOX_SYMBOL = 'O';
constexpr char ROBOT_SYMBOL = '@';
constexpr char WALL_SYMBOL = '#';
constexpr char EMPTY_SYMBOL = '.';
constexpr char LEFT_BOX_SYMBOL = '[';
constexpr char RIGHT_BOX_SYMBOL = ']';

static auto parse_input()
    -> std::pair<std::vector<std::vector<char>>, std::string> {
    const std::string input_file_name{"data/day15.txt"};
    std::ifstream input_file{input_file_name};

    std::vector<std::vector<char>> board;
    std::string attempted_moves;

    std::string line;
    bool read_empty_line = false;
    while(std::getline(input_file, line)) {
        if(line.empty()) {
            read_empty_line = true;
            continue;
        }

        if(!read_empty_line) {
            board.emplace_back(line.begin(), line.end());
        } else {
            attempted_moves += line;
        }
    }

    return std::pair<std::vector<std::vector<char>>, std::string>(
        board, attempted_moves);
}

auto widen_input(const std::vector<std::vector<char>> &board)
    -> std::vector<std::vector<char>> {
    std::vector<std::vector<char>> new_board;
    for(const std::vector<char> &vec : board) {
        std::vector<char> wider_line;
        for(const char c : vec) {
            if(c == WALL_SYMBOL) {
                wider_line.push_back(WALL_SYMBOL);
                wider_line.push_back(WALL_SYMBOL);
            } else if(c == BOX_SYMBOL) {
                wider_line.push_back(LEFT_BOX_SYMBOL);
                wider_line.push_back(RIGHT_BOX_SYMBOL);
            } else if(c == EMPTY_SYMBOL) {
                wider_line.push_back(EMPTY_SYMBOL);
                wider_line.push_back(EMPTY_SYMBOL);
            } else if(c == ROBOT_SYMBOL) {
                wider_line.push_back(ROBOT_SYMBOL);
                wider_line.push_back(EMPTY_SYMBOL);
            }
        }
        new_board.push_back(wider_line);
    }

    return new_board;
}

class Warehouse {
  public:
    explicit Warehouse(std::vector<std::vector<char>> board_)
        : board(std::move(board_)),
          robot_position(std::pair<std::size_t, std::size_t>(0, 0)) {
        robot_position = find_robot_position();
    }

    auto find_robot_position() -> std::pair<std::size_t, std::size_t> {
        for(std::size_t i = 0; i < board.size(); ++i) {
            for(std::size_t j = 0; j < board[i].size(); ++j) {
                if(board[i][j] == ROBOT_SYMBOL) {
                    return std::pair<std::size_t, std::size_t>(i, j);
                }
            }
        }

        throw std::invalid_argument("Did not find ROBOT_SYMBOL in board.");
    }

    auto attempt_to_move(const std::pair<int, int> direction) -> bool {
        const std::pair<int, int> proposed_robot_position(
            static_cast<int>(robot_position.first) + direction.first,
            static_cast<int>(robot_position.second) + direction.second);
        if(!valid_position(proposed_robot_position.first,
                           proposed_robot_position.second)) {
            throw std::runtime_error(
                "Invalid movement; robot is trying to move off of the board");
        }

        bool robot_can_actually_move = true;
        std::set<std::pair<std::size_t, std::size_t>> affected_spaces;
        std::set<std::pair<int, int>> affected_spaces_to_check_neighbors_of{
            std::pair<int, int>(static_cast<int>(robot_position.first),
                                static_cast<int>(robot_position.second))};
        std::set<std::pair<int, int>>
            next_affected_spaces_to_check_neighbors_of;

        while(!affected_spaces_to_check_neighbors_of.empty()) {
            for(const std::pair<int, int> affected_space_to_check_neighbors_of :
                affected_spaces_to_check_neighbors_of) {
                const std::pair<int, int> proposed_next_space(
                    affected_space_to_check_neighbors_of.first +
                        direction.first,
                    affected_space_to_check_neighbors_of.second +
                        direction.second);
                if(!valid_position(proposed_next_space.first,
                                   proposed_next_space.second)) {
                    throw std::runtime_error(
                        "Invalid movement; trying to push off of the board");
                }
                if(board[static_cast<std::size_t>(proposed_next_space.first)]
                        [static_cast<std::size_t>(
                            proposed_next_space.second)] == WALL_SYMBOL) {
                    robot_can_actually_move = false;
                    break;
                }
                if(board[static_cast<std::size_t>(proposed_next_space.first)]
                        [static_cast<std::size_t>(
                            proposed_next_space.second)] == BOX_SYMBOL) {
                    next_affected_spaces_to_check_neighbors_of.emplace(
                        proposed_next_space.first, proposed_next_space.second);
                }
                if(board[static_cast<std::size_t>(proposed_next_space.first)]
                        [static_cast<std::size_t>(
                            proposed_next_space.second)] == LEFT_BOX_SYMBOL) {
                    next_affected_spaces_to_check_neighbors_of.emplace(
                        proposed_next_space.first, proposed_next_space.second);

                    if(direction.second == 0) {
                        next_affected_spaces_to_check_neighbors_of.emplace(
                            proposed_next_space.first,
                            proposed_next_space.second + 1);
                    }
                }
                if(board[static_cast<std::size_t>(proposed_next_space.first)]
                        [static_cast<std::size_t>(
                            proposed_next_space.second)] == RIGHT_BOX_SYMBOL) {
                    next_affected_spaces_to_check_neighbors_of.emplace(
                        proposed_next_space.first, proposed_next_space.second);
                    if(direction.second == 0) {
                        next_affected_spaces_to_check_neighbors_of.emplace(
                            proposed_next_space.first,
                            proposed_next_space.second - 1);
                    }
                }
            }

            if(!robot_can_actually_move) {
                break;
            }

            affected_spaces.insert(
                affected_spaces_to_check_neighbors_of.begin(),
                affected_spaces_to_check_neighbors_of.end());
            affected_spaces_to_check_neighbors_of.swap(
                next_affected_spaces_to_check_neighbors_of);
            next_affected_spaces_to_check_neighbors_of.clear();
        }

        if(robot_can_actually_move) {
            std::vector<std::vector<char>> new_board(board);
            for(const std::pair<int, int> affected_space : affected_spaces) {
                new_board[static_cast<std::size_t>(affected_space.first)]
                         [static_cast<std::size_t>(affected_space.second)] =
                             EMPTY_SYMBOL;
            }
            for(const std::pair<int, int> affected_space : affected_spaces) {
                const std::pair<int, int> next_space(
                    affected_space.first + direction.first,
                    affected_space.second + direction.second);
                new_board[static_cast<std::size_t>(
                    next_space.first)][static_cast<std::size_t>(next_space
                                                                    .second)] =
                    board[static_cast<std::size_t>(affected_space.first)]
                         [static_cast<std::size_t>(affected_space.second)];
            }

            board.swap(new_board);
            robot_position = proposed_robot_position;
        }

        return robot_can_actually_move;
    }

    auto show_board() -> void {
        for(const std::vector<char> &vec : board) {
            for(const char c : vec) {
                std::cout << c;
            }
            std::cout << std::endl;
        }
    }

    auto run_proposed_movements(const std::string &proposed_movements) -> void {
        const std::map<char, std::pair<int, int>> direction_map{
            {'^', std::pair<int, int>(-1, 0)},
            {'>', std::pair<int, int>(0, 1)},
            {'v', std::pair<int, int>(1, 0)},
            {'<', std::pair<int, int>(0, -1)},
        };

        for(const char direction_input : proposed_movements) {
            const std::pair<int, int> movement =
                direction_map.at(direction_input);
            attempt_to_move(movement);
        }
    }

    static auto compute_goods_position_system_coordinate(const std::size_t row,
                                                         const std::size_t col)
        -> int {
        const int row_multiplier = 100;
        return row_multiplier * static_cast<int>(row) + static_cast<int>(col);
    }

    auto compute_total_sum_of_goods_position_system_of_all_boxes() -> int64_t {
        int64_t total_sum_of_goods_position_system_coordinate_of_all_boxes = 0;
        for(std::size_t i = 0; i < board.size(); ++i) {
            for(std::size_t j = 0; j < board[i].size(); ++j) {
                if(board[i][j] == BOX_SYMBOL ||
                   board[i][j] == LEFT_BOX_SYMBOL) {
                    total_sum_of_goods_position_system_coordinate_of_all_boxes +=
                        compute_goods_position_system_coordinate(i, j);
                }
            }
        }

        return total_sum_of_goods_position_system_coordinate_of_all_boxes;
    }

    auto valid_position(const int row, const int col) -> bool {
        return (0 <= row && row < static_cast<int>(board.size()) && 0 <= col &&
                col < static_cast<int>(
                          board[static_cast<std::size_t>(row)].size()));
    }

  private:
    std::vector<std::vector<char>> board;
    std::pair<std::size_t, std::size_t> robot_position;
};

} // namespace Day15

auto solve_day15a() -> int64_t {
    const std::pair<std::vector<std::vector<char>>, std::string> parsed_input =
        Day15::parse_input();
    Day15::Warehouse warehouse(parsed_input.first);
    warehouse.run_proposed_movements(parsed_input.second);
    return warehouse.compute_total_sum_of_goods_position_system_of_all_boxes();
}

auto solve_day15b() -> int64_t {
    const std::pair<std::vector<std::vector<char>>, std::string> parsed_input =
        Day15::parse_input();
    const std::vector<std::vector<char>> wider_board =
        Day15::widen_input(parsed_input.first);
    const std::string proposed_movements = parsed_input.second;
    Day15::Warehouse warehouse(wider_board);
    warehouse.run_proposed_movements(parsed_input.second);
    return warehouse.compute_total_sum_of_goods_position_system_of_all_boxes();
}
