#include <algorithm> // std::min, std::replace
#include <cstdint>   // std::size_t, int64_t
#include <fstream>   // std::ifstream
#include <limits>    // std::numeric_limits
#include <queue>
#include <string>  // std::getline
#include <tuple>   // std::get
#include <utility> // std::pair
#include <vector>

#include "day.hpp"

namespace Day20 {

constexpr char START_SYMBOL = 'S';
constexpr char END_SYMBOL = 'E';
constexpr char WALL_SYMBOL = '#';
constexpr char EMPTY_SYMBOL = '.';

auto parse_input() -> std::tuple<std::vector<std::vector<char>>,
                                 std::pair<std::size_t, std::size_t>,
                                 std::pair<std::size_t, std::size_t>> {
    const std::string input_file_name = "data/day20.txt";
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

auto find_neighbors(const std::pair<std::size_t, std::size_t> location,
                    const std::vector<std::vector<char>> &board)
    -> std::vector<std::pair<std::size_t, std::size_t>> {
    std::vector<std::pair<std::size_t, std::size_t>> neighbors;

    const std::vector<int> dx{0, 1, 0, -1};
    const std::vector<int> dy{1, 0, -1, 0};
    for(std::size_t k = 0; k < dx.size(); ++k) {
        const int candidate_row = static_cast<int>(location.first) + dx[k];
        const int candidate_col = static_cast<int>(location.second) + dy[k];
        if(0 <= candidate_row && 0 <= candidate_col) {
            const auto row = static_cast<std::size_t>(candidate_row);
            const auto col = static_cast<std::size_t>(candidate_col);
            if(row < board.size() && col < board[row].size() &&
               board[row][col] == EMPTY_SYMBOL) {
                neighbors.emplace_back(row, col);
            }
        }
    }

    return neighbors;
}

auto bfs(const std::vector<std::vector<char>> &board,
         const std::pair<std::size_t, std::size_t> &start)
    -> std::vector<std::vector<std::size_t>> {
    std::vector<std::vector<bool>> visited(
        board.size(), std::vector<bool>(board[0].size(), false));
    std::vector<std::vector<std::size_t>> distances(
        board.size(),
        std::vector<std::size_t>(board[0].size(),
                                 std::numeric_limits<std::size_t>::max()));
    distances[start.first][start.second] = 0;

    std::queue<std::pair<std::size_t, std::size_t>> q;
    q.push(start);

    while(!q.empty()) {
        const std::pair<std::size_t, std::size_t> curr = q.front();
        q.pop();

        if(visited[curr.first][curr.second]) {
            continue;
        }
        visited[curr.first][curr.second] = true;

        const std::vector<std::pair<std::size_t, std::size_t>> neighbors =
            find_neighbors(curr, board);
        for(const std::pair<std::size_t, std::size_t> &neighbor : neighbors) {
            if(!visited[neighbor.first][neighbor.second]) {
                distances[neighbor.first][neighbor.second] =
                    std::min(1 + distances[curr.first][curr.second],
                             distances[neighbor.first][neighbor.second]);
                q.push(neighbor);
            }
        }
    }

    return distances;
}

auto find_cheats(const std::vector<std::vector<char>> &board)
    -> std::vector<std::pair<std::pair<std::size_t, std::size_t>,
                             std::pair<std::size_t, std::size_t>>> {
    std::vector<std::pair<std::pair<std::size_t, std::size_t>,
                          std::pair<std::size_t, std::size_t>>>
        cheats;

    const std::vector<int> dx{0, 2, 0, -2};
    const std::vector<int> dy{2, 0, -2, 0};

    for(std::size_t row = 0; row < board.size(); ++row) {
        for(std::size_t col = 0; col < board[row].size(); ++col) {
            if(board[row][col] != EMPTY_SYMBOL) {
                continue;
            }

            for(std::size_t k = 0; k < dx.size(); ++k) {
                const int candidate_row2 = static_cast<int>(row) + dx[k];
                const int candidate_col2 = static_cast<int>(col) + dy[k];
                if(0 <= candidate_row2 && 0 <= candidate_col2) {
                    const auto row2 = static_cast<std::size_t>(candidate_row2);
                    const auto col2 = static_cast<std::size_t>(candidate_col2);
                    if(row2 < board.size() && col2 < board[row2].size()) {

                        const std::size_t between_row = (row + row2) / 2;
                        const std::size_t between_col = (col + col2) / 2;

                        if(board[between_row][between_col] == WALL_SYMBOL &&
                           board[row2][col2] == EMPTY_SYMBOL) {
                            cheats.emplace_back(
                                std::pair<std::size_t, std::size_t>(row, col),
                                std::pair<std::size_t, std::size_t>(row2,
                                                                    col2));
                        }
                    }
                }
            }
        }
    }

    return cheats;
}

auto find_num_cheats_that_save_time(
    const std::vector<std::vector<char>> &board,
    const std::pair<std::size_t, std::size_t> &start,
    const std::pair<std::size_t, std::size_t> &end, const int64_t time_to_save)
    -> int64_t {
    int64_t num_cheats = 0;
    const std::vector<std::pair<std::pair<std::size_t, std::size_t>,
                                std::pair<std::size_t, std::size_t>>>
        cheats = find_cheats(board);
    const std::vector<std::vector<std::size_t>> distances_from_start =
        bfs(board, start);
    const std::vector<std::vector<std::size_t>> distances_from_end =
        bfs(board, end);
    const std::size_t time_to_finish_without_cheats =
        distances_from_start[end.first][end.second];
    const std::size_t time_to_cheat = 2;

    for(const std::pair<std::pair<std::size_t, std::size_t>,
                        std::pair<std::size_t, std::size_t>> &cheat_locations :
        cheats) {
        const std::pair<std::size_t, std::size_t> cheat_start =
            cheat_locations.first;
        const std::pair<std::size_t, std::size_t> cheat_end =
            cheat_locations.second;
        const std::size_t time_to_finish_with_cheat =
            distances_from_start[cheat_start.first][cheat_start.second] +
            time_to_cheat +
            distances_from_end[cheat_end.first][cheat_end.second];
        const int64_t time_saved =
            static_cast<int64_t>(time_to_finish_without_cheats) -
            static_cast<int64_t>(time_to_finish_with_cheat);
        if(time_saved >= time_to_save) {
            ++num_cheats;
        }
    }

    return num_cheats;
}

} // namespace Day20

auto solve_day20a() -> int64_t {
    const std::tuple<std::vector<std::vector<char>>,
                     std::pair<std::size_t, std::size_t>,
                     std::pair<std::size_t, std::size_t>>
        inputs = Day20::parse_input();
    const std::vector<std::vector<char>> &board = std::get<0>(inputs);
    const std::pair<std::size_t, std::size_t> start_location =
        std::get<1>(inputs);
    const std::pair<std::size_t, std::size_t> end_location =
        std::get<2>(inputs);
    const int64_t time_to_save = 100;
    return Day20::find_num_cheats_that_save_time(board, start_location,
                                                 end_location, time_to_save);
}

auto solve_day20b() -> int64_t { return 0; }