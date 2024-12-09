#include <cstddef>
#include <fstream>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "day.hpp"

constexpr const char FREE = '.';
constexpr const char OBSTACLE = '#';
constexpr const char START = '^';

struct WalkResult {
    unsigned int num_steps;
    std::set<std::pair<std::size_t, std::size_t>> visited_positions;
    std::set<
        std::pair<std::pair<std::size_t, std::size_t>, std::pair<int, int>>>
        visited_position_with_direction;
    bool in_loop;
};

static auto parse_input() -> std::vector<std::string> {
    const std::string input_file_name{"data/day06.txt"};
    std::ifstream input_file{input_file_name};

    std::vector<std::string> grid;
    std::string line;
    while(std::getline(input_file, line)) {
        grid.push_back(line);
    }

    return grid;
}

auto valid_next_position(const std::size_t num_rows, const std::size_t num_cols,
                         const std::size_t row, const std::size_t col,
                         const std::pair<int, int> direction) -> bool {
    bool valid_row = true;
    if(direction.first == -1) {
        valid_row = (row > 0);
    } else if(direction.first == 1) {
        valid_row = (row + 1 < num_rows);
    }

    bool valid_col = true;
    if(direction.second == -1) {
        valid_col = (col > 0);
    } else if(direction.second == 1) {
        valid_col = (col + 1 < num_cols);
    }
    return valid_row && valid_col;
}

auto next_position(const std::size_t pos, const int direction) -> std::size_t {
    if(direction == 1) {
        return pos + 1;
    }
    if(direction == -1) {
        return pos - 1;
    }

    return pos;
}

auto turn_right(const std::pair<int, int> current_direction)
    -> std::pair<int, int> {
    return std::pair<int, int>(current_direction.second,
                               -current_direction.first);
}

auto find_start_position(const std::vector<std::string> &grid)
    -> std::pair<std::size_t, std::size_t> {
    for(std::size_t i = 0; i < grid.size(); ++i) {
        for(std::size_t j = 0; j < grid[i].size(); ++j) {
            if(grid[i][j] == START) {
                return std::pair(i, j);
            }
        }
    }

    return std::pair(0, 0);
}

/**
 * Walk in direction from starting_position in grid, until an obstacle is hit,
 * or hit edge.
 */
auto walk(const std::vector<std::string> &grid,
          const std::pair<std::size_t, std::size_t> starting_position,
          const std::pair<int, int> starting_direction) -> WalkResult {

    unsigned int num_steps = 0;
    std::set<std::pair<std::size_t, std::size_t>> visited_positions{
        starting_position};
    std::set<
        std::pair<std::pair<std::size_t, std::size_t>, std::pair<int, int>>>
        visited_position_with_direction{
            std::pair(starting_position, starting_direction)};
    bool in_loop = false;

    std::size_t current_row = starting_position.first;
    std::size_t current_col = starting_position.second;
    std::pair<int, int> direction = starting_direction;

    while(valid_next_position(grid.size(), grid[0].size(), current_row,
                              current_col, direction)) {
        const std::size_t next_row =
            next_position(current_row, direction.first);
        const std::size_t next_col =
            next_position(current_col, direction.second);
        if(grid[next_row][next_col] == OBSTACLE) {
            direction = turn_right(direction);

            const std::pair<std::pair<std::size_t, std::size_t>,
                            std::pair<int, int>>
                next_position_with_direction =
                    std::pair(std::pair(current_row, current_col), direction);

            if(visited_position_with_direction.find(
                   next_position_with_direction) !=
               visited_position_with_direction.end()) {
                in_loop = true;
                break;
            }
            visited_position_with_direction.insert(
                next_position_with_direction);
            continue;
        }

        current_row = next_row;
        current_col = next_col;
        num_steps++;
        const std::pair<std::size_t, std::size_t> current_position_after_moving(
            current_row, current_col);
        visited_positions.insert(current_position_after_moving);
        const std::pair<std::pair<std::size_t, std::size_t>,
                        std::pair<int, int>>
            current_position_with_direction_after_moving =
                std::pair(current_position_after_moving, direction);
        if(visited_position_with_direction.find(
               current_position_with_direction_after_moving) !=
           visited_position_with_direction.end()) {
            in_loop = true;
            break;
        }
        visited_position_with_direction.insert(
            current_position_with_direction_after_moving);
    }

    return WalkResult{num_steps, visited_positions,
                      visited_position_with_direction, in_loop};
}

auto solve_day06a() -> int64_t {
    const std::vector<std::string> grid = parse_input();
    const std::pair<std::size_t, std::size_t> starting_position =
        find_start_position(grid);
    const WalkResult walk_result =
        walk(grid, starting_position, std::pair(-1, 0));
    const std::set<std::pair<std::size_t, std::size_t>> visited_positions =
        walk_result.visited_positions;

    const auto result = static_cast<int64_t>(visited_positions.size());
    return result;
}

auto solve_day06b() -> int64_t {
    std::vector<std::string> grid = parse_input();
    const std::pair<std::size_t, std::size_t> starting_position =
        find_start_position(grid);
    const WalkResult initial_walk_result =
        walk(grid, starting_position, std::pair(-1, 0));
    const std::set<std::pair<std::size_t, std::size_t>>
        initial_visited_positions = initial_walk_result.visited_positions;

    int64_t num_positions_that_lead_to_loop = 0;
    // try placing an obstacle at each visited location, except for starting
    // position
    for(const std::pair<std::size_t, std::size_t> visited_position :
        initial_visited_positions) {
        const std::size_t potential_row = visited_position.first;
        const std::size_t potential_col = visited_position.second;
        // cannot place obstacle at guard's starting position, or if there
        // already is an obstacle
        if(grid[potential_row][potential_col] != FREE) {
            continue;
        }

        // replace with obstacle
        grid[potential_row].replace(potential_col, 1, 1, OBSTACLE);
        const WalkResult walk_result =
            walk(grid, starting_position, std::pair(-1, 0));
        if(walk_result.in_loop) {
            ++num_positions_that_lead_to_loop;
        }

        // undo: replace spot with free space
        grid[potential_row].replace(potential_col, 1, 1, FREE);
    }

    return num_positions_that_lead_to_loop;
}
