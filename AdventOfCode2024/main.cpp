#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "day.hpp"

auto solve(const std::size_t day, const std::string &problem_version)
    -> int64_t {
    const std::size_t star_index =
        2 * (day - 1) + ((problem_version == "B") ? 1 : 0);
    const std::vector<int64_t (*)()> problem_to_solver{
        &solve_day01a, &solve_day01b, &solve_day02a, &solve_day02b,
        &solve_day03a, &solve_day03b, &solve_day04a, &solve_day04b,
        &solve_day05a, &solve_day05b, &solve_day06a, &solve_day06b,
        &solve_day07a, &solve_day07b, &solve_day08a, &solve_day08b,
        &solve_day09a, &solve_day09b, &solve_day10a, &solve_day10b,
        &solve_day11a, &solve_day11b, &solve_day12a, &solve_day12b,
        &solve_day13a, &solve_day13b, &solve_day14a, &solve_day14b,
        &solve_day15a, &solve_day15b, &solve_day16a, &solve_day16b,
        &solve_day17a, &solve_day17b, &solve_day18a, &solve_day18b,
        &solve_day19a, &solve_day19b, &solve_day20a, &solve_day20b,
        &solve_day21a, &solve_day21b, &solve_day22a, &solve_day22b,
        &solve_day23a, &solve_day23b, &solve_day24a, &solve_day24b};

    int64_t (*const specific_solver_function)() =
        problem_to_solver.at(star_index);
    return specific_solver_function();
}

auto main(int argc, char *argv[]) -> int {
    const std::vector<std::string> args(argv, argv + argc);
    if(argc != 3) {
        std::cerr << "Parameters day number and 'A'/'B' is required"
                  << std::endl;
        return 1;
    }
    const std::string which_day{args[1]};
    const std::string which_problem{args[2]};

    if(which_problem != "A" && which_problem != "B") {
        std::cerr << "Argument must be either 'A' or 'B'" << std::endl;
        return 1;
    }

    const int day_number = std::stoi(which_day);
    if(day_number <= 0) {
        std::cerr << "Day number must be at least 1" << std::endl;
        return 1;
    }
    const int64_t result = solve(std::size_t(day_number), which_problem);

    std::cout << result << std::endl;
    return 0;
}
