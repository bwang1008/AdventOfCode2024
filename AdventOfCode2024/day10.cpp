#include <cstdint>
#include <fstream>
#include <queue>
#include <string>
#include <utility>
#include <vector>

#include "day.hpp"

constexpr const int BASE = 10;

static auto parse_input() -> std::vector<std::vector<int>> {
    const std::string input_file_name{"data/day10.txt"};
    std::ifstream input_file{input_file_name};

    std::vector<std::vector<int>> input;
    std::string line;
    while(std::getline(input_file, line)) {
        std::vector<int> row(line.size());
        for(std::size_t i = 0; i < line.size(); ++i) {
            row[i] = (line[i] - '0');
        }
        input.push_back(row);
    }
    return input;
}

auto score_individual_0(const std::vector<std::vector<int>> &grid,
                        const std::size_t starting_row,
                        const std::size_t starting_col) -> int {
    const std::size_t R = grid.size();
    const std::size_t C = grid[0].size();
    std::vector<std::vector<bool>> visited(R, std::vector<bool>(C, false));
    int num9 = 0;

    std::queue<std::pair<std::size_t, std::size_t>> q;
    q.emplace(starting_row, starting_col);

    while(!q.empty()) {
        const std::pair<std::size_t, std::size_t> position = q.front();
        q.pop();
        const std::size_t row = position.first;
        const std::size_t col = position.second;

        if(visited[row][col]) {
            continue;
        }
        visited[row][col] = true;
        if(grid[row][col] == BASE - 1) {
            ++num9;
            continue;
        }

        if(0 < row && row < R + 1 && col < C) {
            if(!visited[row - 1][col] &&
               grid[row - 1][col] == grid[row][col] + 1) {
                q.emplace(row - 1, col);
            }
        }
        if(row + 1 < R && col < C) {
            if(!visited[row + 1][col] &&
               grid[row + 1][col] == grid[row][col] + 1) {
                q.emplace(row + 1, col);
            }
        }
        if(row < R && 0 < col && col < C + 1) {
            if(!visited[row][col - 1] &&
               grid[row][col - 1] == grid[row][col] + 1) {
                q.emplace(row, col - 1);
            }
        }
        if(row < R && col + 1 < C) {
            if(!visited[row][col + 1] &&
               grid[row][col + 1] == grid[row][col] + 1) {
                q.emplace(row, col + 1);
            }
        }
    }

    return num9;
}

auto solve_day10a() -> int64_t {
    const std::vector<std::vector<int>> grid = parse_input();

    int64_t total_score = 0;
    for(std::size_t i = 0; i < grid.size(); ++i) {
        for(std::size_t j = 0; j < grid[i].size(); ++j) {
            if(grid[i][j] == 0) {
                total_score += score_individual_0(grid, i, j);
            }
        }
    }

    return total_score;
}

auto rate_every_cell(const std::vector<std::vector<int>> &grid)
    -> std::vector<std::vector<int>> {
    const std::size_t R = grid.size();
    const std::size_t C = grid[0].size();
    std::vector<std::vector<int>> scores(R, std::vector<int>(C, -1));
    std::vector<std::vector<bool>> visited(R, std::vector<bool>(C, false));

    std::queue<std::pair<std::size_t, std::size_t>> q;
    for(std::size_t i = 0; i < R; ++i) {
        for(std::size_t j = 0; j < C; ++j) {
            if(grid[i][j] == BASE - 1) {
                q.emplace(i, j);
            }
        }
    }

    while(!q.empty()) {
        const std::pair<std::size_t, std::size_t> position = q.front();
        q.pop();
        const std::size_t row = position.first;
        const std::size_t col = position.second;

        if(visited[row][col]) {
            continue;
        }
        visited[row][col] = true;

        int sum = 0;
        if(0 < row && row < R + 1 && col < C) {
            if(grid[row - 1][col] == grid[row][col] + 1 &&
               scores[row - 1][col] >= 0) {
                sum += scores[row - 1][col];
            }
            if(!visited[row - 1][col] &&
               grid[row - 1][col] == grid[row][col] - 1) {
                q.emplace(row - 1, col);
            }
        }
        if(row + 1 < R && col < C) {
            if(grid[row + 1][col] == grid[row][col] + 1 &&
               scores[row + 1][col] >= 0) {
                sum += scores[row + 1][col];
            }
            if(!visited[row + 1][col] &&
               grid[row + 1][col] == grid[row][col] - 1) {
                q.emplace(row + 1, col);
            }
        }
        if(row < R && 0 < col && col < C + 1) {
            if(grid[row][col - 1] == grid[row][col] + 1 &&
               scores[row][col - 1] >= 0) {
                sum += scores[row][col - 1];
            }
            if(!visited[row][col - 1] &&
               grid[row][col - 1] == grid[row][col] - 1) {
                q.emplace(row, col - 1);
            }
        }
        if(row < R && col + 1 < C) {
            if(grid[row][col + 1] == grid[row][col] + 1 &&
               scores[row][col + 1] >= 0) {
                sum += scores[row][col + 1];
            }
            if(!visited[row][col + 1] &&
               grid[row][col + 1] == grid[row][col] - 1) {
                q.emplace(row, col + 1);
            }
        }

        if(grid[row][col] == BASE - 1) {
            sum = 1;
        }
        scores[row][col] = sum;
    }

    return scores;
}

auto solve_day10b() -> int64_t {
    const std::vector<std::vector<int>> grid = parse_input();
    const std::vector<std::vector<int>> scores = rate_every_cell(grid);

    int64_t total_score = 0;
    for(std::size_t i = 0; i < grid.size(); ++i) {
        for(std::size_t j = 0; j < grid[i].size(); ++j) {
            if(grid[i][j] == 0 && scores[i][j] >= 0) {
                total_score += scores[i][j];
            }
        }
    }

    return total_score;
}