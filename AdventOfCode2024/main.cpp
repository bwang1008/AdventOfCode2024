#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "day.hpp"

auto solve(const int day, const std::string &problem_version) -> int64_t {
    const std::map<std::pair<int, std::string>, int64_t (*)()>
        problem_to_solver{
            {std::pair<int, std::string>(1, "A"), &solve_day01a},
            {std::pair<int, std::string>(1, "B"), &solve_day01b},
            {std::pair<int, std::string>(2, "A"), &solve_day02a},
            {std::pair<int, std::string>(2, "B"), &solve_day02b},
            {std::pair<int, std::string>(3, "A"), &solve_day03a},
            {std::pair<int, std::string>(3, "B"), &solve_day03b},
        };

    int64_t (*const specific_solver_function)() =
        problem_to_solver.at(std::pair<int, std::string>(day, problem_version));
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

    const int64_t result = solve(std::stoi(which_day), which_problem);

    std::cout << result << std::endl;
    return 0;
}
