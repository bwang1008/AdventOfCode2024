#include <cstdint>
#include <fstream>
#include <regex>
#include <string>
#include <utility>
#include <vector>

#include "day.hpp"

constexpr const int BATHROOM_WIDTH = 101;
constexpr const int BATHROOM_HEIGHT = 103;
constexpr const int NUM_QUADRANTS = 4;

struct RestroomRobot {
    int initial_col_position;
    int initial_row_position;
    int col_velocity;
    int row_velocity;
};

static auto parse_input() -> std::vector<RestroomRobot> {
    const std::string input_file_name{"data/day14.txt"};
    std::ifstream input_file{input_file_name};

    const std::regex retrieve_robot_info(
        R"(p=(-?\d+),(-?\d+) v=(-?\d+),(-?\d+))");

    std::vector<RestroomRobot> restroom_robots;
    std::string line;
    while(std::getline(input_file, line)) {
        std::smatch match_result;
        std::regex_match(line, match_result, retrieve_robot_info);

        restroom_robots.push_back(RestroomRobot{
            .initial_col_position = std::stoi(match_result[1].str()),
            .initial_row_position = std::stoi(match_result[2].str()),
            .col_velocity = std::stoi(match_result[3].str()),
            .row_velocity = std::stoi(match_result[4].str())});
    }

    return restroom_robots;
}

auto find_position_after_n_seconds(const RestroomRobot restroom_robot,
                                   const int num_seconds)
    -> std::pair<int, int> {
    int final_col = (restroom_robot.initial_col_position +
                     num_seconds * restroom_robot.col_velocity) %
                    BATHROOM_WIDTH;
    int final_row = (restroom_robot.initial_row_position +
                     num_seconds * restroom_robot.row_velocity) %
                    BATHROOM_HEIGHT;

    if(final_col < 0) {
        final_col += BATHROOM_WIDTH;
    }
    if(final_row < 0) {
        final_row += BATHROOM_HEIGHT;
    }
    return std::pair(final_col, final_row);
}

auto find_quadrant_of_position(const int col, const int row) -> std::size_t {
    if(row == BATHROOM_HEIGHT / 2 || col == BATHROOM_WIDTH / 2) {
        return 0;
    }
    if(row > BATHROOM_HEIGHT / 2) {
        if(col > BATHROOM_WIDTH / 2) {
            return 4;
        }
        return 3;
    }
    if(col > BATHROOM_WIDTH / 2) {
        return 1;
    }
    return 2;
}

auto solve_day14a() -> int64_t {
    constexpr const int NUM_SECONDS = 100;
    const std::vector<RestroomRobot> restroom_robots = parse_input();
    std::vector<int> quadrant_count(NUM_QUADRANTS + 1, 0);
    for(const RestroomRobot restroom_robot : restroom_robots) {
        const std::pair<int, int> final_position =
            find_position_after_n_seconds(restroom_robot, NUM_SECONDS);
        const std::size_t quadrant = find_quadrant_of_position(
            final_position.first, final_position.second);
        quadrant_count[quadrant]++;
    }
    const int result = quadrant_count[1] * quadrant_count[2] *
                       quadrant_count[3] * quadrant_count[4];
    return result;
}

auto solve_day14b() -> int64_t {
    const int result = 0;
    return result;
}