#include <cstddef>
#include <fstream>
#include <string>
#include <vector>

#include "day.hpp"

auto is_valid_position(const std::vector<std::string> &grid, const int row,
                       const int col) -> bool {
    return 0 <= row && std::size_t(row) < grid.size() && 0 <= col &&
           std::size_t(col) < grid[std::size_t(row)].size();
}

auto word_exists_at_position_in_direction(const std::vector<std::string> &grid,
                                          const std::string &pattern,
                                          const int starting_row,
                                          const int starting_col, const int dx,
                                          const int dy) -> bool {
    for(int i = 0; std::size_t(i) < pattern.size(); ++i) {
        const int row = starting_row + i * dx;
        const int col = starting_col + i * dy;
        if(!is_valid_position(grid, row, col) ||
           grid[std::size_t(row)][std::size_t(col)] !=
               pattern[std::size_t(i)]) {
            return false;
        }
    }

    return true;
}

static auto parse_input() -> std::vector<std::string> {
    const std::string input_file_name{"data/day04.txt"};
    std::ifstream input_file{input_file_name};

    std::vector<std::string> input;
    std::string line;
    while(std::getline(input_file, line)) {
        input.push_back(line);
    }

    return input;
}

auto solve_day04a() -> int64_t {
    const std::vector<std::string> grid = parse_input();
    const std::string search_word = "XMAS";
    int64_t count = 0;
    for(int i = 0; std::size_t(i) < grid.size(); ++i) {
        for(int j = 0; std::size_t(j) < grid[std::size_t(i)].size(); ++j) {
            for(int dx = -1; dx <= 1; ++dx) {
                for(int dy = -1; dy <= 1; ++dy) {
                    if(dx == 0 && dy == 0) {
                        continue;
                    }

                    if(word_exists_at_position_in_direction(grid, search_word,
                                                            i, j, dx, dy)) {
                        ++count;
                    }
                }
            }
        }
    }

    return count;
}

auto forms_double_mas(const std::vector<std::string> &grid,
                      const std::size_t starting_row,
                      const std::size_t starting_col) -> bool {
    if(grid[starting_row][starting_col] != 'A') {
        return false;
    }

    // all corners comprised of 2 M and 2 S
    unsigned int num_m = 0;
    unsigned int num_s = 0;
    const char corner1 = grid[starting_row - 1][starting_col - 1];
    const char corner2 = grid[starting_row - 1][starting_col + 1];
    const char corner3 = grid[starting_row + 1][starting_col - 1];
    const char corner4 = grid[starting_row + 1][starting_col + 1];
    const std::vector<char> corner_letters{corner1, corner2, corner3, corner4};
    for(const char corner : corner_letters) {
        if(corner == 'M') {
            num_m++;
        } else if(corner == 'S') {
            num_s++;
        }
    }

    if(!(num_m == 2 && num_s == 2)) {
        return false;
    }

    return corner1 != corner4;
}

auto solve_day04b() -> int64_t {
    const std::vector<std::string> grid = parse_input();
    int64_t count = 0;
    for(std::size_t i = 1; i + 1 < grid.size(); ++i) {
        for(std::size_t j = 1; j + 1 < grid[i].size(); ++j) {
            if(forms_double_mas(grid, i, j)) {
                ++count;
            }
        }
    }

    return count;
}
