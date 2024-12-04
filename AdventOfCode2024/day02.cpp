#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

constexpr const int INCREASE_LOWER_BOUND = 1;
constexpr const int INCREASE_UPPER_BOUND = 3;

auto split(const std::string &text, const char delimiter)
    -> std::vector<std::string> {
    std::vector<std::string> tokens;
    std::size_t last = 0;
    std::size_t next = 0;
    while((next = text.find(delimiter, last)) != std::string::npos) {
        tokens.push_back(text.substr(last, next - last));
        last = next + 1;
    }
    tokens.push_back(text.substr(last));

    return tokens;
}

auto parse_input() -> std::vector<std::vector<int>> {
    const std::string input_file_name{"data/day02.txt"};
    std::ifstream input_file{input_file_name};

    std::vector<std::vector<int>> input;
    std::string line;
    while(std::getline(input_file, line)) {
        const std::vector<std::string> tokens = split(line, ' ');
        std::vector<int> line_ints(tokens.size());
        for(std::size_t i = 0; i < tokens.size(); ++i) {
            const std::string token = tokens[i];
            line_ints[i] = std::stoi(token);
        }
        input.push_back(line_ints);
    }

    return input;
}

auto report_is_safe(const std::vector<int> &report) -> bool {
    if(report.size() < 2) {
        return true;
    }

    const bool initial_increasing = ((report[1] - report[0]) > 0);
    for(std::size_t i = 1; i < report.size(); ++i) {
        const int diff = report[i] - report[i - 1];
        const bool currently_increasing = (diff > 0);

        if(!(initial_increasing == currently_increasing &&
             INCREASE_LOWER_BOUND <= std::abs(diff) &&
             std::abs(diff) <= INCREASE_UPPER_BOUND)) {
            return false;
        }
    }

    return true;
}

auto solve_a() -> int64_t {
    const std::vector<std::vector<int>> reports = parse_input();
    int64_t count = 0;
    for(const std::vector<int> &report : reports) {
        if(report_is_safe(report)) {
            ++count;
        }
    }

    return count;
}

auto solve_b() -> int64_t { return 0; }

auto main(int argc, char *argv[]) -> int {
    const std::vector<std::string> args(argv, argv + argc);
    if(argc != 2) {
        std::cerr << "An argument 'A' or 'B' is required" << std::endl;
        return 1;
    }
    const std::string which_problem{args[1]};

    int64_t result = 0;
    if(which_problem == "A") {
        result = solve_a();
    } else if(which_problem == "B") {
        result = solve_b();
    } else {
        std::cerr << "Argument must be either 'A' or 'B'" << std::endl;
        return 1;
    }

    std::cout << result << std::endl;
    return 0;
}
