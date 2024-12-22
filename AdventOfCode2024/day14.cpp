#include <cstdint>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <utility>
#include <vector>

#include "day.hpp"

constexpr const int BATHROOM_WIDTH = 101;
constexpr const int BATHROOM_HEIGHT = 103;
constexpr const int NUM_QUADRANTS = 4;

class RestroomRobot {
  public:
    RestroomRobot(const int initial_col_position_,
                  const int initial_row_position_, const int col_velocity_,
                  const int row_velocity_)
        : initial_col_position(initial_col_position_),
          initial_row_position(initial_row_position_),
          current_col_position(initial_col_position_),
          current_row_position(initial_row_position_),
          col_velocity(col_velocity_), row_velocity(row_velocity_) {}
    [[nodiscard]] auto get_current_position() const -> std::pair<int, int> {
        return std::pair(current_col_position, current_row_position);
    }
    auto step(const int num_seconds) -> void {
        int final_col = (current_col_position + num_seconds * col_velocity) %
                        BATHROOM_WIDTH;
        int final_row = (current_row_position + num_seconds * row_velocity) %
                        BATHROOM_HEIGHT;
        if(final_col < 0) {
            final_col += BATHROOM_WIDTH;
        }
        if(final_row < 0) {
            final_row += BATHROOM_HEIGHT;
        }

        current_col_position = final_col;
        current_row_position = final_row;
    }
    [[nodiscard]] auto find_quadrant() const -> std::size_t {
        if(current_row_position == BATHROOM_HEIGHT / 2 ||
           current_col_position == BATHROOM_WIDTH / 2) {
            return 0;
        }
        if(current_row_position > BATHROOM_HEIGHT / 2) {
            if(current_col_position > BATHROOM_WIDTH / 2) {
                return 4;
            }
            return 3;
        }
        if(current_col_position > BATHROOM_WIDTH / 2) {
            return 1;
        }
        return 2;
    }

  private:
    int initial_col_position;
    int initial_row_position;
    int current_col_position;
    int current_row_position;
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

        restroom_robots.emplace_back(
            std::stoi(match_result[1].str()), std::stoi(match_result[2].str()),
            std::stoi(match_result[3].str()), std::stoi(match_result[4].str()));
    }

    return restroom_robots;
}

auto print_robot_positions(const std::vector<RestroomRobot> &robots) -> void {
    std::vector<std::vector<bool>> visited(
        BATHROOM_HEIGHT, std::vector<bool>(BATHROOM_WIDTH, false));
    for(const auto robot : robots) {
        const std::pair<int, int> current_position =
            robot.get_current_position();
        visited[static_cast<std::size_t>(current_position.second)]
               [static_cast<std::size_t>(current_position.first)] = true;
    }
    for(const auto &row : visited) {
        for(const bool is_visited : row) {
            if(is_visited) {
                std::cout << 'R';
            } else {
                std::cout << '.';
            }
        }
        std::cout << std::endl;
    }
}

auto find_percentage_of_robot_positions_have_at_least_one_neighbor(
    const std::vector<RestroomRobot> &robots) -> double {
    std::vector<std::vector<bool>> visited(
        BATHROOM_HEIGHT, std::vector<bool>(BATHROOM_WIDTH, false));
    for(const auto robot : robots) {
        const std::pair<int, int> current_position =
            robot.get_current_position();
        visited[static_cast<std::size_t>(current_position.second)]
               [static_cast<std::size_t>(current_position.first)] = true;
    }

    int number_of_robot_positions = 0;
    for(const auto &row : visited) {
        for(const bool is_visited : row) {
            if(is_visited) {
                ++number_of_robot_positions;
            }
        }
    }

    const std::vector<int> dx{-1, -1, -1, 0, 0, 1, 1, 1};
    const std::vector<int> dy{-1, 0, 1, -1, 1, -1, 0, 1};

    int num_have_neighbors = 0;
    for(std::size_t i = 0; i < visited.size(); ++i) {
        for(std::size_t j = 0; j < visited[i].size(); ++j) {
            if(!visited[i][j]) {
                continue;
            }

            bool has_neighbors = false;
            for(std::size_t k = 0; k < dx.size(); ++k) {
                int dx1 = dx[k];
                int dy1 = dy[k];

                std::size_t neighbor_row =
                    (i + static_cast<std::size_t>(BATHROOM_HEIGHT + dx1)) %
                    BATHROOM_HEIGHT;
                std::size_t neighbor_col =
                    (j + static_cast<std::size_t>(BATHROOM_WIDTH + dy1)) %
                    BATHROOM_WIDTH;

                if(visited[neighbor_row][neighbor_col]) {
                    has_neighbors = true;
                    break;
                }
            }

            if(has_neighbors) {
                ++num_have_neighbors;
            }
        }
    }

    return num_have_neighbors / static_cast<double>(number_of_robot_positions);
}

auto solve_day14a() -> int64_t {
    constexpr const int NUM_SECONDS = 100;
    std::vector<RestroomRobot> restroom_robots = parse_input();
    std::vector<int> quadrant_count(NUM_QUADRANTS + 1, 0);
    for(RestroomRobot &restroom_robot : restroom_robots) {
        restroom_robot.step(NUM_SECONDS);
        const std::size_t quadrant = restroom_robot.find_quadrant();
        quadrant_count[quadrant]++;
    }
    const int result = quadrant_count[1] * quadrant_count[2] *
                       quadrant_count[3] * quadrant_count[4];
    return result;
}

auto solve_day14b() -> int64_t {
    constexpr const int VERIFIED_SECONDS = 0;
    constexpr const int MAX_SECONDS_TO_TRY = 30000;
    constexpr const double NEIGHBOR_TOLERANCE = 0.7;
    constexpr const int LOGGING_FREQUENCY = 200;

    std::vector<RestroomRobot> restroom_robots = parse_input();
    for(RestroomRobot &restroom_robot : restroom_robots) {
        restroom_robot.step(VERIFIED_SECONDS);
    }

    int num_seconds = 1 + VERIFIED_SECONDS;
    while(num_seconds < MAX_SECONDS_TO_TRY) {

        if(num_seconds % LOGGING_FREQUENCY == 0) {
            std::cout << "num_seconds at " << num_seconds << std::endl;
        }

        for(RestroomRobot &restroom_robot : restroom_robots) {
            restroom_robot.step(1);
        }

        const double percentage_have_neighbors =
            find_percentage_of_robot_positions_have_at_least_one_neighbor(
                restroom_robots);
        if(percentage_have_neighbors > NEIGHBOR_TOLERANCE) {
            std::cout << "On second " << num_seconds
                      << ", perentage of positions have neighbors: "
                      << percentage_have_neighbors << std::endl;
            std::cout << "Robots look like:" << std::endl;
            print_robot_positions(restroom_robots);
            break;
        }

        num_seconds += 1;
    }

    return num_seconds;
}