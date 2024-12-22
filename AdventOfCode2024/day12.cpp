#include <cstdint>
#include <fstream>
#include <queue>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "day.hpp"

static auto parse_input() -> std::vector<std::string> {
    const std::string input_file_name{"data/day12.txt"};
    std::ifstream input_file{input_file_name};

    std::vector<std::string> grid;
    std::string line;
    while(std::getline(input_file, line)) {
        grid.push_back(line);
    }

    return grid;
}

auto fill_in_region_id(const std::vector<std::string> &grid,
                       std::vector<std::vector<int>> &grid_region_ids,
                       std::vector<std::vector<bool>> &visited,
                       const std::size_t starting_row,
                       const std::size_t starting_col, const int region_id)
    -> void {
    std::queue<std::pair<std::size_t, std::size_t>> q;
    q.emplace(starting_row, starting_col);

    while(!q.empty()) {
        const std::pair<std::size_t, std::size_t> current_position = q.front();
        q.pop();

        const std::size_t current_row = current_position.first;
        const std::size_t current_col = current_position.second;

        if(visited[current_row][current_col]) {
            continue;
        }
        visited[current_row][current_col] = true;
        grid_region_ids[current_row][current_col] = region_id;

        if(0 < current_row &&
           grid[current_row - 1][current_col] ==
               grid[current_row][current_col] &&
           !visited[current_row - 1][current_col]) {
            q.emplace(current_row - 1, current_col);
        }
        if(current_row + 1 < grid.size() &&
           grid[current_row + 1][current_col] ==
               grid[current_row][current_col] &&
           !visited[current_row + 1][current_col]) {
            q.emplace(current_row + 1, current_col);
        }
        if(0 < current_col &&
           grid[current_row][current_col - 1] ==
               grid[current_row][current_col] &&
           !visited[current_row][current_col - 1]) {
            q.emplace(current_row, current_col - 1);
        }
        if(current_col + 1 < grid[current_row].size() &&
           grid[current_row][current_col + 1] ==
               grid[current_row][current_col] &&
           !visited[current_row][current_col + 1]) {
            q.emplace(current_row, current_col + 1);
        }
    }
}

auto partition_grid(const std::vector<std::string> &grid)
    -> std::vector<std::vector<int>> {
    const std::size_t R = grid.size();
    const std::size_t C = grid[0].size();

    std::vector<std::vector<int>> grid_region_ids(R, std::vector<int>(C, -1));
    std::vector<std::vector<bool>> visited(R, std::vector<bool>(C, false));
    int region_id = 0;

    for(std::size_t i = 0; i < R; ++i) {
        for(std::size_t j = 0; j < C; ++j) {
            if(!visited[i][j]) {
                fill_in_region_id(grid, grid_region_ids, visited, i, j,
                                  region_id);
                ++region_id;
            }
        }
    }

    return grid_region_ids;
}

auto find_area(const std::vector<std::vector<int>> &grid_region_ids)
    -> std::unordered_map<int, int> {
    std::unordered_map<int, int> region_id_to_area;

    for(const auto &row : grid_region_ids) {
        for(const auto region_id : row) {
            region_id_to_area[region_id]++;
        }
    }

    return region_id_to_area;
}

auto find_perimeter(const std::vector<std::vector<int>> &grid_region_ids)
    -> std::unordered_map<int, int> {
    std::unordered_map<int, int> region_id_to_perimeter;

    for(std::size_t i = 0; i < grid_region_ids.size(); ++i) {
        for(std::size_t j = 0; j < grid_region_ids[i].size(); ++j) {
            const int region_id = grid_region_ids[i][j];

            int perimeter_length_to_add = 4;
            if(0 < i && grid_region_ids[i - 1][j] == region_id) {
                perimeter_length_to_add -= 2;
            }
            if(0 < j && grid_region_ids[i][j - 1] == region_id) {
                perimeter_length_to_add -= 2;
            }
            region_id_to_perimeter[region_id] += perimeter_length_to_add;
        }
    }

    return region_id_to_perimeter;
}

auto find_number_of_edges(const std::vector<std::vector<int>> &grid_region_ids)
    -> std::unordered_map<int, int> {
    std::unordered_map<int, int> region_id_to_edges;

    for(std::size_t i = 0; i < grid_region_ids.size(); ++i) {
        for(std::size_t j = 0; j < grid_region_ids[i].size(); ++j) {
            const int region_id = grid_region_ids[i][j];

            bool exists_top_left =
                (0 < i && 0 < j && grid_region_ids[i - 1][j - 1] == region_id);
            bool exists_top = (0 < i && grid_region_ids[i - 1][j] == region_id);
            bool exists_left =
                (0 < j && grid_region_ids[i][j - 1] == region_id);
            bool exists_top_right =
                (0 < i && j + 1 < grid_region_ids[i - 1].size() &&
                 grid_region_ids[i - 1][j + 1] == region_id);

            int perimeter_length_to_add = 0;

            if(exists_top_left) {
                if(exists_top) {
                    if(exists_top_right) {
                        if(exists_left) {
                            perimeter_length_to_add = 0;
                        } else {
                            perimeter_length_to_add = 4;
                        }
                    } else {
                        if(exists_left) {
                            perimeter_length_to_add = -2;
                        } else {
                            perimeter_length_to_add = 2;
                        }
                    }
                } else {
                    if(exists_left) {
                        perimeter_length_to_add = 2;
                    } else {
                        perimeter_length_to_add = 4;
                    }
                }
            } else {
                if(exists_top) {
                    if(exists_top_right) {
                        if(exists_left) {
                            perimeter_length_to_add = 0;
                        } else {
                            perimeter_length_to_add = 2;
                        }
                    } else {
                        if(exists_left) {
                            perimeter_length_to_add = -2;
                        } else {
                            perimeter_length_to_add = 0;
                        }
                    }
                } else {
                    if(exists_left) {
                        perimeter_length_to_add = 0;
                    } else {
                        perimeter_length_to_add = 4;
                    }
                }
            }

            region_id_to_edges[region_id] += perimeter_length_to_add;
        }
    }

    return region_id_to_edges;
}

auto find_total_fencing_price(
    const std::unordered_map<int, int> &region_id_to_area,
    const std::unordered_map<int, int> &region_id_to_perimeter) -> int64_t {
    int64_t total_price = 0;
    for(const auto region_id_and_area : region_id_to_area) {
        const int region_id = region_id_and_area.first;
        const int area = region_id_and_area.second;
        const int perimeter = region_id_to_perimeter.at(region_id);
        total_price += area * perimeter;
    }

    return total_price;
}

auto solve_day12a() -> int64_t {
    const std::vector<std::string> grid = parse_input();
    const std::vector<std::vector<int>> grid_region_ids = partition_grid(grid);
    const std::unordered_map<int, int> region_id_to_area =
        find_area(grid_region_ids);
    const std::unordered_map<int, int> region_id_to_perimeter =
        find_perimeter(grid_region_ids);

    return find_total_fencing_price(region_id_to_area, region_id_to_perimeter);
}

auto solve_day12b() -> int64_t {
    const std::vector<std::string> grid = parse_input();
    const std::vector<std::vector<int>> grid_region_ids = partition_grid(grid);
    const std::unordered_map<int, int> region_id_to_area =
        find_area(grid_region_ids);
    const std::unordered_map<int, int> region_id_to_edges =
        find_number_of_edges(grid_region_ids);

    return find_total_fencing_price(region_id_to_area, region_id_to_edges);
}