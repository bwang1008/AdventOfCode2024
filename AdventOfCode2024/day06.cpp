#include <algorithm>
#include <cstddef>
#include <fstream>
#include <functional>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "day.hpp"

constexpr const char FREE = '.';
constexpr const char OBSTACLE = '#';
constexpr const char START = '^';

enum class StopReason {
    None,
    Obstacle,
    Edge,
    Loop,
};

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

auto get_obstacle_locations(const std::vector<std::string> &grid)
    -> std::pair<std::unordered_map<std::size_t, std::set<std::size_t>>,
                 std::unordered_map<std::size_t, std::set<std::size_t>>> {
    std::unordered_map<std::size_t, std::set<std::size_t>> obstacles_row_view;
    std::unordered_map<std::size_t, std::set<std::size_t>> obstacles_col_view;
    for(std::size_t i = 0; i < grid.size(); ++i) {
        for(std::size_t j = 0; j < grid[i].size(); ++j) {
            if(grid[i][j] == OBSTACLE) {
                obstacles_row_view[i].insert(j);
                obstacles_col_view[j].insert(i);
            }
        }
    }

    return std::pair(obstacles_row_view, obstacles_col_view);
}

/**
 * Given a current (row, col) position and direction to walk in, return the last
 * position in the line it can walk in.
 */
auto bunny_hop(const std::vector<std::string> &grid,
               const std::pair<std::size_t, std::size_t> starting_position,
               const std::pair<int, int> direction,
               const std::unordered_map<std::size_t, std::set<std::size_t>>
                   &obstacles_row_view,
               const std::unordered_map<std::size_t, std::set<std::size_t>>
                   &obstacles_col_view

               ) -> std::pair<std::pair<std::size_t, std::size_t>, StopReason> {
    const std::size_t current_row = starting_position.first;
    const std::size_t current_col = starting_position.second;

    if(direction.first == -1 && direction.second == 0) {
        // heading up
        if(obstacles_col_view.find(current_col) == obstacles_col_view.end()) {
            return std::pair(std::pair(0, current_col), StopReason::Edge);
        }
        const std::set<std::size_t> &rows_of_this_column =
            obstacles_col_view.at(current_col);
        const auto next_obstacle = std::upper_bound(
            rows_of_this_column.rbegin(), rows_of_this_column.rend(),
            current_row, std::greater<>{});
        if(next_obstacle == rows_of_this_column.rend()) {
            return std::pair(std::pair(0, current_col), StopReason::Edge);
        }
        const std::size_t next_row = *next_obstacle + 1;
        return std::pair(std::pair(next_row, current_col),
                         StopReason::Obstacle);
    }

    if(direction.first == 1 && direction.second == 0) {
        // heading down
        if(obstacles_col_view.find(current_col) == obstacles_col_view.end()) {
            return std::pair(std::pair(grid.size() - 1, current_col),
                             StopReason::Edge);
        }
        const std::set<std::size_t> &rows_of_this_column =
            obstacles_col_view.at(current_col);
        const auto next_obstacle =
            std::upper_bound(rows_of_this_column.begin(),
                             rows_of_this_column.end(), current_row);
        if(next_obstacle == rows_of_this_column.end()) {
            return std::pair(std::pair(grid.size() - 1, current_col),
                             StopReason::Edge);
        }
        const std::size_t next_row = *next_obstacle - 1;
        return std::pair(std::pair(next_row, current_col),
                         StopReason::Obstacle);
    }

    if(direction.first == 0 && direction.second == -1) {
        // heading left
        if(obstacles_row_view.find(current_row) == obstacles_row_view.end()) {
            return std::pair(std::pair(current_row, 0), StopReason::Edge);
        }
        const std::set<std::size_t> &cols_of_this_row =
            obstacles_row_view.at(current_row);
        const auto next_obstacle =
            std::upper_bound(cols_of_this_row.rbegin(), cols_of_this_row.rend(),
                             current_col, std::greater<>{});
        if(next_obstacle == cols_of_this_row.rend()) {
            return std::pair(std::pair(current_row, 0), StopReason::Edge);
        }
        const std::size_t next_col = *next_obstacle + 1;
        return std::pair(std::pair(current_row, next_col),
                         StopReason::Obstacle);
    }

    if(direction.first == 0 && direction.second == 1) {
        // heading right
        if(obstacles_row_view.find(current_row) == obstacles_row_view.end()) {
            return std::pair(
                std::pair(current_row, grid[current_row].size() - 1),
                StopReason::Edge);
        }
        const std::set<std::size_t> &cols_of_this_row =
            obstacles_row_view.at(current_row);
        const auto next_obstacle = std::upper_bound(
            cols_of_this_row.begin(), cols_of_this_row.end(), current_col);
        if(next_obstacle == cols_of_this_row.end()) {
            return std::pair(
                std::pair(current_row, grid[current_row].size() - 1),
                StopReason::Edge);
        }
        const std::size_t next_col = *next_obstacle - 1;
        return std::pair(std::pair(current_row, next_col),
                         StopReason::Obstacle);
    }

    return std::pair(std::pair(current_row, current_col), StopReason::Obstacle);
}

auto bunny_hop_until_the_end(
    const std::vector<std::string> &grid,
    const std::pair<std::size_t, std::size_t> starting_position,
    const std::pair<int, int> direction,
    const std::unordered_map<std::size_t, std::set<std::size_t>>
        &obstacles_row_view,
    const std::unordered_map<std::size_t, std::set<std::size_t>>
        &obstacles_col_view) -> StopReason {
    std::set<
        std::pair<std::pair<std::size_t, std::size_t>, std::pair<int, int>>>
        visited;
    std::size_t current_row = starting_position.first;
    std::size_t current_col = starting_position.second;
    std::pair<int, int> current_direction = direction;

    StopReason current_stop_reason = StopReason::Obstacle;
    while(current_stop_reason == StopReason::Obstacle) {
        visited.insert(
            std::pair(std::pair(current_row, current_col), current_direction));
        const std::pair<std::pair<std::size_t, std::size_t>, StopReason>
            next_location = bunny_hop(grid, std::pair(current_row, current_col),
                                      current_direction, obstacles_row_view,
                                      obstacles_col_view);

        if(next_location.second == StopReason::Edge) {
            return StopReason::Edge;
        }
        const std::pair<int, int> next_direction =
            turn_right(current_direction);
        const std::pair<std::pair<std::size_t, std::size_t>,
                        std::pair<int, int>>
            next_state = std::pair(next_location.first, next_direction);
        if(visited.find(next_state) != visited.end()) {
            return StopReason::Loop;
        }
        current_row = next_location.first.first;
        current_col = next_location.first.second;
        current_direction = next_direction;
    }

    return StopReason::Loop;
}

auto walk_while_placing_obstacles(
    std::vector<std::string> &grid,
    const std::pair<std::size_t, std::size_t> starting_position,
    const std::pair<int, int> starting_direction,
    std::unordered_map<std::size_t, std::set<std::size_t>> obstacles_row_view,
    std::unordered_map<std::size_t, std::set<std::size_t>> obstacles_col_view)
    -> int {
    int num_obstacles_lead_to_loop = 0;
    std::set<std::pair<std::size_t, std::size_t>> obstacle_positions_placed;
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
            continue;
        }

        if(obstacle_positions_placed.find(std::pair(next_row, next_col)) ==
           obstacle_positions_placed.end()) {
            // next space is free: try placing an obstacle
            grid[next_row][next_col] = OBSTACLE;
            obstacle_positions_placed.insert(std::pair(next_row, next_col));
            obstacles_row_view[next_row].insert(next_col);
            obstacles_col_view[next_col].insert(next_row);
            const StopReason future_scenario = bunny_hop_until_the_end(
                grid, std::pair(current_row, current_col),
                turn_right(direction), obstacles_row_view, obstacles_col_view);
            if(future_scenario == StopReason::Loop) {
                ++num_obstacles_lead_to_loop;
            }

            // remove obstacle
            grid[next_row][next_col] = FREE;
            obstacles_row_view[next_row].erase(next_col);
            obstacles_col_view[next_col].erase(next_row);
        }

        // walk one step forward
        current_row = next_row;
        current_col = next_col;
    }

    return num_obstacles_lead_to_loop;
}

auto solve_day06b() -> int64_t {
    std::vector<std::string> grid = parse_input();
    const std::pair<std::size_t, std::size_t> starting_position =
        find_start_position(grid);

    const std::pair<std::unordered_map<std::size_t, std::set<std::size_t>>,
                    std::unordered_map<std::size_t, std::set<std::size_t>>>
        obstacle_locations = get_obstacle_locations(grid);
    std::unordered_map<std::size_t, std::set<std::size_t>> obstacles_row_view =
        obstacle_locations.first;
    std::unordered_map<std::size_t, std::set<std::size_t>> obstacles_col_view =
        obstacle_locations.second;

    return walk_while_placing_obstacles(grid, starting_position,
                                        std::pair(-1, 0), obstacles_row_view,
                                        obstacles_col_view);
}
