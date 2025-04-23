#include <algorithm> // std::min
#include <cstdint>   // std::size_t, int64_t
#include <fstream>   // std::ifstream
#include <iostream>  // std::cout, std::endl
#include <limits>    // std::numeric_limits
#include <queue>
#include <sstream> // std::stringstream
#include <string>  // std::getline
#include <utility> // std::pair
#include <vector>

#include "day.hpp"

namespace Day18 {

constexpr std::size_t MAX_ROWS = 71;
constexpr std::size_t MAX_COLS = 71;

auto parse_input() -> std::vector<std::pair<std::size_t, std::size_t>> {
    const std::string input_file_name = "data/day18.txt";
    std::ifstream input_file{input_file_name};

    std::vector<std::pair<std::size_t, std::size_t>> result;
    std::string line;
    while(std::getline(input_file, line)) {
        const std::size_t comma_index = line.find(',');
        std::size_t column = 0;
        std::size_t row = 0;
        std::stringstream ss;
        ss << line.substr(0, comma_index);
        ss >> column;
        ss.clear();
        ss << line.substr(comma_index + 1, line.size() - comma_index - 1);
        ss >> row;
        result.emplace_back(row, column);
    }

    return result;
}

auto get_is_obstacle(
    const std::vector<std::pair<std::size_t, std::size_t>> &obstacle_locations,
    const std::size_t num_obstacles) -> std::vector<std::vector<bool>> {
    std::vector<std::vector<bool>> is_obstacle(
        MAX_ROWS, std::vector<bool>(MAX_COLS, false));
    for(std::size_t i = 0; i < num_obstacles; ++i) {
        const std::pair<std::size_t, std::size_t> position =
            obstacle_locations[i];
        is_obstacle[position.first][position.second] = true;
    }
    return is_obstacle;
}

auto print_obstacles(const std::vector<std::vector<bool>> &is_obstacle)
    -> void {
    for(const std::vector<bool> &vec : is_obstacle) {
        for(const bool b : vec) {
            std::cout << (b ? '#' : '.');
        }
        std::cout << std::endl;
    }
}

auto bfs(const std::vector<std::vector<bool>> &is_obstacle,
         const std::pair<std::size_t, std::size_t> start_position,
         const std::pair<std::size_t, std::size_t> end_position)
    -> std::size_t {
    const std::vector<int> dx{1, 0, -1, 0};
    const std::vector<int> dy{0, 1, 0, -1};

    std::vector<std::vector<bool>> visited(MAX_ROWS,
                                           std::vector<bool>(MAX_COLS, false));
    std::vector<std::vector<std::size_t>> distances(
        MAX_ROWS, std::vector<std::size_t>(
                      MAX_COLS, std::numeric_limits<std::size_t>::max()));
    distances[start_position.first][start_position.second] = 0;

    std::queue<std::pair<std::size_t, std::size_t>> q;
    q.push(start_position);

    while(!q.empty()) {
        const std::pair<std::size_t, std::size_t> curr = q.front();
        q.pop();

        if(visited[curr.first][curr.second]) {
            continue;
        }
        visited[curr.first][curr.second] = true;

        for(std::size_t k = 0; k < dx.size(); ++k) {
            const int potential_next_row = static_cast<int>(curr.first) + dx[k];
            const int potential_next_col =
                static_cast<int>(curr.second) + dy[k];
            if(0 <= potential_next_row &&
               static_cast<std::size_t>(potential_next_row) < MAX_ROWS &&
               0 <= potential_next_col &&
               static_cast<std::size_t>(potential_next_col) < MAX_COLS) {
                const auto next_row =
                    static_cast<std::size_t>(potential_next_row);
                const auto next_col =
                    static_cast<std::size_t>(potential_next_col);
                if(!is_obstacle[next_row][next_col] &&
                   !visited[next_row][next_col]) {
                    distances[next_row][next_col] =
                        std::min(distances[curr.first][curr.second] + 1,
                                 distances[next_row][next_col]);
                    q.emplace(next_row, next_col);
                }
            }
        }
    }

    return distances[end_position.first][end_position.second];
}

} // namespace Day18

auto solve_day18a() -> int64_t {
    const std::vector<std::pair<std::size_t, std::size_t>> obstacles =
        Day18::parse_input();
    const std::size_t num_obstacles = 1024;
    const std::vector<std::vector<bool>> is_obstacle =
        Day18::get_is_obstacle(obstacles, num_obstacles);
    return static_cast<int64_t>(
        Day18::bfs(is_obstacle, std::pair<std::size_t, std::size_t>(0, 0),
                   std::pair<std::size_t, std::size_t>(Day18::MAX_ROWS - 1,
                                                       Day18::MAX_COLS - 1)));
}

auto solve_day18b() -> int64_t {
    const std::vector<std::pair<std::size_t, std::size_t>> obstacles =
        Day18::parse_input();

    int low = 0;
    int high = static_cast<int>(obstacles.size());
    while(low <= high) {
        const int mid = (low + high) / 2;
        const std::vector<std::vector<bool>> is_obstacle =
            Day18::get_is_obstacle(obstacles, static_cast<std::size_t>(mid));
        const std::size_t path_length =
            Day18::bfs(is_obstacle, std::pair<std::size_t, std::size_t>(0, 0),
                       std::pair<std::size_t, std::size_t>(
                           Day18::MAX_ROWS - 1, Day18::MAX_COLS - 1));
        if(path_length == std::numeric_limits<std::size_t>::max()) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    const auto min_obstacles_that_block = static_cast<std::size_t>(low);
    const std::size_t row = obstacles[min_obstacles_that_block - 1].first;
    const std::size_t col = obstacles[min_obstacles_that_block - 1].second;
    std::cout << col << "," << row << std::endl;
    return 0;
}
