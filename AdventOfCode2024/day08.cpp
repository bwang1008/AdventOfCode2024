#include <cstddef>
#include <fstream>
#include <numeric> // std::gcd
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "day.hpp"

constexpr const char NON_OBSTACLE = '.';

static auto parse_input() -> std::vector<std::string> {
    const std::string input_file_name{"data/day08.txt"};
    std::ifstream input_file{input_file_name};

    std::vector<std::string> grid;
    std::string line;
    while(std::getline(input_file, line)) {
        grid.push_back(line);
    }

    return grid;
}

auto parse_antenna_locations(const std::vector<std::string> &grid)
    -> std::unordered_map<char,
                          std::vector<std::pair<std::size_t, std::size_t>>> {

    std::unordered_map<char, std::vector<std::pair<std::size_t, std::size_t>>>
        antenna_locations;
    for(std::size_t row = 0; row < grid.size(); ++row) {
        for(std::size_t col = 0; col < grid[row].size(); ++col) {
            if(grid[row][col] == NON_OBSTACLE) {
                continue;
            }

            antenna_locations[grid[row][col]].push_back(std::pair(row, col));
        }
    }

    return antenna_locations;
}

auto find_antinodes(const std::vector<std::string> &grid,
                    const std::unordered_map<
                        char, std::vector<std::pair<std::size_t, std::size_t>>>
                        &antenna_locations)
    -> std::unordered_map<char,
                          std::vector<std::pair<std::size_t, std::size_t>>> {

    std::unordered_map<char, std::vector<std::pair<std::size_t, std::size_t>>>
        antinode_locations;
    for(const std::pair<const char,
                        std::vector<std::pair<std::size_t, std::size_t>>>
            &frequency_and_antenna_locations : antenna_locations) {
        const char frequency = frequency_and_antenna_locations.first;
        const std::vector<std::pair<std::size_t, std::size_t>>
            single_frequency_antenna_locations =
                frequency_and_antenna_locations.second;

        for(std::pair<std::size_t, std::size_t> antenna1 :
            single_frequency_antenna_locations) {

            for(std::pair<std::size_t, std::size_t> antenna2 :
                single_frequency_antenna_locations) {
                if(antenna1.first == antenna2.first &&
                   antenna1.second == antenna2.second) {
                    continue;
                }
                // Let dx = x2 - x1
                // want x3 = x2 + dx == x2 + x2 - x1
                // but only if:
                // 0 <= x3   :   0 <= 2*x2 - x1, or x1 <= 2x2
                // x3 < some size  :   2x2 < x1 + grid.size()
                if(antenna1.first <= 2 * antenna2.first &&
                   2 * antenna2.first < grid.size() + antenna1.first &&
                   antenna1.second <= 2 * antenna2.second &&
                   2 * antenna2.second <
                       grid[2 * antenna2.first - antenna1.first].size() +
                           antenna1.second) {
                    const std::size_t antinode_row =
                        2 * antenna2.first - antenna1.first;
                    const std::size_t antinode_col =
                        2 * antenna2.second - antenna1.second;

                    antinode_locations[frequency].push_back(
                        std::pair(antinode_row, antinode_col));
                }
            }
        }
    }

    return antinode_locations;
}

auto solve_day08a() -> int64_t {
    const std::vector<std::string> grid = parse_input();
    const std::unordered_map<char,
                             std::vector<std::pair<std::size_t, std::size_t>>>
        antenna_locations = parse_antenna_locations(grid);
    const std::unordered_map<char,
                             std::vector<std::pair<std::size_t, std::size_t>>>
        antinode_locations = find_antinodes(grid, antenna_locations);

    std::set<std::pair<std::size_t, std::size_t>> unique_antinode_locations;
    for(const std::pair<const char,
                        std::vector<std::pair<std::size_t, std::size_t>>>
            &frequency_and_antinode_locations : antinode_locations) {
        const std::vector<std::pair<std::size_t, std::size_t>>
            single_frequency_antinode_locations =
                frequency_and_antinode_locations.second;
        for(const std::pair<std::size_t, std::size_t> antinode_location :
            single_frequency_antinode_locations) {
            unique_antinode_locations.insert(antinode_location);
        }
    }

    return static_cast<int64_t>(unique_antinode_locations.size());
}

auto find_antinodes_with_resonant_harmonics(
    const std::vector<std::string> &grid,
    const std::unordered_map<char,
                             std::vector<std::pair<std::size_t, std::size_t>>>
        &antenna_locations) -> std::set<std::pair<std::size_t, std::size_t>> {

    std::set<std::pair<std::size_t, std::size_t>> antinode_locations;
    for(const std::pair<const char,
                        std::vector<std::pair<std::size_t, std::size_t>>>
            &frequency_and_antenna_locations : antenna_locations) {
        const std::vector<std::pair<std::size_t, std::size_t>>
            single_frequency_antenna_locations =
                frequency_and_antenna_locations.second;

        for(std::size_t i = 0; i < single_frequency_antenna_locations.size();
            ++i) {
            const int antenna1_row =
                static_cast<int>(single_frequency_antenna_locations[i].first);
            const int antenna1_col =
                static_cast<int>(single_frequency_antenna_locations[i].second);

            for(std::size_t j = i + 1;
                j < single_frequency_antenna_locations.size(); ++j) {
                const int antenna2_row = static_cast<int>(
                    single_frequency_antenna_locations[j].first);
                const int antenna2_col = static_cast<int>(
                    single_frequency_antenna_locations[j].second);

                const int dx = antenna2_row - antenna1_row;
                const int dy = antenna2_col - antenna1_col;
                const int g = std::gcd(dx, dy);
                const int drow = dx / g;
                const int dcol = dy / g;

                for(int antinode_row = antenna1_row,
                        antinode_col = antenna1_col;
                    0 <= antinode_row &&
                    static_cast<std::size_t>(antinode_row) < grid.size() &&
                    0 <= antinode_col &&
                    static_cast<std::size_t>(antinode_col) <
                        grid[static_cast<std::size_t>(antinode_row)].size();
                    antinode_row += drow, antinode_col += dcol) {
                    antinode_locations.insert(
                        std::pair(static_cast<std::size_t>(antinode_row),
                                  static_cast<std::size_t>(antinode_col)));
                }
                for(int antinode_row = antenna1_row,
                        antinode_col = antenna1_col;
                    0 <= antinode_row &&
                    static_cast<std::size_t>(antinode_row) < grid.size() &&
                    0 <= antinode_col &&
                    static_cast<std::size_t>(antinode_col) <
                        grid[static_cast<std::size_t>(antinode_row)].size();
                    antinode_row -= drow, antinode_col -= dcol) {
                    antinode_locations.insert(
                        std::pair(static_cast<std::size_t>(antinode_row),
                                  static_cast<std::size_t>(antinode_col)));
                }
            }
        }
    }

    return antinode_locations;
}

auto solve_day08b() -> int64_t {
    const std::vector<std::string> grid = parse_input();
    const std::unordered_map<char,
                             std::vector<std::pair<std::size_t, std::size_t>>>
        antenna_locations = parse_antenna_locations(grid);
    const std::set<std::pair<std::size_t, std::size_t>> antinode_locations =
        find_antinodes_with_resonant_harmonics(grid, antenna_locations);
    return static_cast<int64_t>(antinode_locations.size());
}
