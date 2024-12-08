#include <cstddef>
#include <fstream>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "day.hpp"

constexpr const char START = '^';
constexpr const char OBSTACLE = '#';

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
          const std::pair<std::size_t, std::size_t> starting_position)
    -> std::set<std::pair<std::size_t, std::size_t>> {

    std::set<std::pair<std::size_t, std::size_t>> visited{starting_position};
    std::pair<int, int> direction(-1, 0);

    std::size_t current_row = starting_position.first;
    std::size_t current_col = starting_position.second;

    while(valid_next_position(grid.size(), grid[0].size(), current_row,
                              current_col, direction)) {
        const std::size_t next_row =
            next_position(current_row, direction.first);
        const std::size_t next_col =
            next_position(current_col, direction.second);
        if(grid[next_row][next_col] == OBSTACLE) {
            direction = turn_right(direction);
            continue;
        }

        current_row = next_row;
        current_col = next_col;
        visited.insert(std::pair(next_row, next_col));
    }

    return visited;
}

auto solve_day06a() -> int64_t {
    const std::vector<std::string> grid = parse_input();
    const std::pair<std::size_t, std::size_t> starting_position =
        find_start_position(grid);
    const std::set<std::pair<std::size_t, std::size_t>> visited_positions =
        walk(grid, starting_position);

    const auto result = static_cast<int64_t>(visited_positions.size());
    return result;
}

auto solve_day06b() -> int64_t { return 0; }
