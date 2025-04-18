#include <algorithm> // std::min, std::replace
#include <cstdint>   // std::size_t, int64_t
#include <fstream>   // std::ifstream
#include <limits>    // std::numeric_limits
#include <map>
#include <queue> // std::priority_queue
#include <set>
#include <tuple>   // std::get
#include <utility> // std::pair
#include <vector>

#include "day.hpp"

namespace Day16 {

constexpr char START_SYMBOL = 'S';
constexpr char END_SYMBOL = 'E';
constexpr char WALL_SYMBOL = '#';
constexpr char EMPTY_SYMBOL = '.';
constexpr int64_t NUMBER_OF_DIFFERENT_DIRECTIONS = 4;
constexpr int64_t SCORE_PER_ROTATION = 1000;
constexpr int64_t SCORE_PER_STEP = 1;

enum Direction : std::size_t { EAST = 0, NORTH = 1, WEST = 2, SOUTH = 3 };

static auto parse_input() -> std::tuple<std::vector<std::vector<char>>,
                                        std::pair<std::size_t, std::size_t>,
                                        std::pair<std::size_t, std::size_t>> {
    const std::string input_file_name = "data/day16.txt";
    std::ifstream input_file{input_file_name};

    std::vector<std::vector<char>> board;
    std::pair<std::size_t, std::size_t> start_location(0, 0);
    std::pair<std::size_t, std::size_t> end_location(0, 0);

    std::size_t row = 0;
    std::string line;

    while(std::getline(input_file, line)) {
        for(std::size_t col = 0; col < line.size(); ++col) {
            if(line[col] == START_SYMBOL) {
                start_location = std::pair<std::size_t, std::size_t>(row, col);
            } else if(line[col] == END_SYMBOL) {
                end_location = std::pair<std::size_t, std::size_t>(row, col);
            }
        }
        std::replace(line.begin(), line.end(), START_SYMBOL, EMPTY_SYMBOL);
        std::replace(line.begin(), line.end(), END_SYMBOL, EMPTY_SYMBOL);
        board.emplace_back(line.begin(), line.end());
        ++row;
    }

    return std::tuple<std::vector<std::vector<char>>,
                      std::pair<std::size_t, std::size_t>,
                      std::pair<std::size_t, std::size_t>>(
        board, start_location, end_location);
}

auto parts_to_index(const std::size_t row, const std::size_t col,
                    const Direction direction, const std::size_t max_cols)
    -> std::size_t {
    return NUMBER_OF_DIFFERENT_DIRECTIONS * (max_cols * row + col) +
           static_cast<std::size_t>(direction);
}

auto index_to_parts(const std::size_t index, const std::size_t max_cols)
    -> std::pair<std::pair<std::size_t, std::size_t>, Direction> {
    const auto direction =
        static_cast<Direction>(index % NUMBER_OF_DIFFERENT_DIRECTIONS);
    const std::size_t row = (index / NUMBER_OF_DIFFERENT_DIRECTIONS) / max_cols;
    const std::size_t col = (index / NUMBER_OF_DIFFERENT_DIRECTIONS) % max_cols;
    return std::pair<std::pair<std::size_t, std::size_t>, Direction>(
        std::pair<std::size_t, std::size_t>(row, col), direction);
}

auto find_neighbors(const std::vector<std::vector<char>> &board,
                    const std::size_t row, const std::size_t col,
                    const Direction direction)
    -> std::vector<std::pair<std::pair<std::size_t, std::size_t>, Direction>> {
    const std::map<Direction, std::pair<int, int>> direction_to_deltas = {
        {Direction::EAST, std::pair<int, int>(0, 1)},
        {Direction::NORTH, std::pair<int, int>(-1, 0)},
        {Direction::WEST, std::pair<int, int>(0, -1)},
        {Direction::SOUTH, std::pair<int, int>(1, 0)},
    };

    std::vector<std::pair<std::pair<std::size_t, std::size_t>, Direction>>
        neighbors;

    // moving forward
    const int next_row =
        static_cast<int>(next_row) + direction_to_deltas.at(direction).first;
    const int next_col =
        static_cast<int>(next_col) + direction_to_deltas.at(direction).second;
    if(0 <= next_row && next_row < board.size() && 0 <= next_col &&
       next_col < board[static_cast<std::size_t>(next_row)].size()) {
        const auto actual_next_row = static_cast<std::size_t>(next_row);
        const auto actual_next_col = static_cast<std::size_t>(next_col);
        if(board[actual_next_row][actual_next_col] == EMPTY_SYMBOL) {
            neighbors.emplace_back(std::pair<std::size_t, std::size_t>(
                                       actual_next_row, actual_next_col),
                                   direction);
        }
    }

    // check other 3 directions
    for(const auto &direction_to_delta : direction_to_deltas) {
        const Direction next_direction = direction_to_delta.first;
        // only move if it is perpendicular
        if((static_cast<std::size_t>(direction) -
            static_cast<std::size_t>(next_direction)) %
               2 !=
           0) {
            neighbors.emplace_back(
                std::pair<std::size_t, std::size_t>(row, col), next_direction);
        }
    }

    return neighbors;
}

auto dijkstra(const std::vector<std::vector<char>> &board,
              const std::size_t start_index)
    -> std::map<std::size_t, std::size_t> {
    std::set<std::size_t> visited;
    std::map<std::size_t, std::size_t> distances;
    distances[start_index] = 0;

    std::priority_queue<std::pair<std::size_t, std::size_t>> pq;
    pq.push(std::pair<std::size_t, std::size_t>(0, start_index));

    while(!pq.empty()) {
        const std::pair<std::size_t, std::size_t> node = pq.top();
        pq.pop();
        const std::size_t current_distance = node.first;
        const std::size_t index = node.second;

        if(visited.find(index) != visited.end()) {
            continue;
        }
        visited.insert(index);

        const std::pair<std::pair<std::size_t, std::size_t>, Direction> parts =
            index_to_parts(index, board[0].size());
        const std::size_t row = parts.first.first;
        const std::size_t col = parts.first.second;
        const Direction direction = parts.second;

        const std::vector<
            std::pair<std::pair<std::size_t, std::size_t>, Direction>>
            neighbors = find_neighbors(board, row, col, direction);
        for(const std::pair<std::pair<std::size_t, std::size_t>, Direction>
                neighbor_parts : neighbors) {
            const std::size_t next_row = neighbor_parts.first.first;
            const std::size_t next_col = neighbor_parts.first.second;
            const Direction next_direction = neighbor_parts.second;
            const std::size_t next_index = parts_to_index(
                next_row, next_col, next_direction, board[0].size());
            if(visited.find(next_index) != visited.end()) {
                continue;
            }

            if(distances.find(next_index) == distances.end()) {
                distances[next_index] = std::numeric_limits<std::size_t>::max();
            }
            const std::size_t next_cost = (direction == next_direction)
                                              ? SCORE_PER_STEP
                                              : SCORE_PER_ROTATION;
            distances[next_index] =
                std::min(current_distance + next_cost, distances[next_index]);
            pq.push(std::pair<std::size_t, std::size_t>(distances[next_index],
                                                        next_index));
        }
    }

    return distances;
}

auto find_min_score(const std::vector<std::vector<char>> &board,
                    const std::pair<std::size_t, std::size_t> start_location,
                    const std::pair<std::size_t, std::size_t> end_location)
    -> int64_t {
    const std::size_t start_index =
        parts_to_index(start_location.first, start_location.second,
                       Direction::EAST, board[0].size());
    const std::size_t end_index_east =
        parts_to_index(end_location.first, end_location.second, Direction::EAST,
                       board[0].size());
    const std::size_t end_index_north =
        parts_to_index(end_location.first, end_location.second,
                       Direction::NORTH, board[0].size());
    const std::size_t end_index_west =
        parts_to_index(end_location.first, end_location.second, Direction::WEST,
                       board[0].size());
    const std::size_t end_index_south =
        parts_to_index(end_location.first, end_location.second,
                       Direction::SOUTH, board[0].size());
    const std::vector<std::size_t> possible_end_indexes{
        end_index_east, end_index_north, end_index_west, end_index_south};

    const std::map<std::size_t, std::size_t> distances =
        dijkstra(board, start_index);

    std::size_t best_distance_at_end = std::numeric_limits<std::size_t>::max();
    for(const std::size_t possible_end_index : possible_end_indexes) {
        if(distances.find(possible_end_index) != distances.end()) {
            best_distance_at_end = std::min(distances.at(possible_end_index),
                                            best_distance_at_end);
        }
    }

    return static_cast<int64_t>(best_distance_at_end);
}

} // namespace Day16

auto solve_day16a() -> int64_t {
    const std::tuple<std::vector<std::vector<char>>,
                     std::pair<std::size_t, std::size_t>,
                     std::pair<std::size_t, std::size_t>>
        inputs = Day16::parse_input();
    const std::vector<std::vector<char>> &board = std::get<0>(inputs);
    const std::pair<std::size_t, std::size_t> start_location =
        std::get<1>(inputs);
    const std::pair<std::size_t, std::size_t> end_location =
        std::get<2>(inputs);
    return Day16::find_min_score(board, start_location, end_location);
}

auto solve_day16b() -> int64_t { return 0; }
