#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

auto parse_input() -> std::pair<std::vector<int>, std::vector<int>> {
    const std::string input_file_name{"data/day01.txt"};
    std::ifstream input_file{input_file_name};

    std::vector<int> list1;
    std::vector<int> list2;
    bool first = true;
    int location_id = 0;

    while(input_file >> location_id) {
        if(first) {
            list1.push_back(location_id);
        } else {
            list2.push_back(location_id);
        }
        first = !first;
    }

    return std::pair<std::vector<int>, std::vector<int>>(list1, list2);
}

auto solve_a() -> int64_t {
    std::pair<std::vector<int>, std::vector<int>> input_data = parse_input();
    std::vector<int> list1 = input_data.first;
    std::vector<int> list2 = input_data.second;

    std::sort(list1.begin(), list1.end());
    std::sort(list2.begin(), list2.end());

    int64_t total_distance = 0;
    for(std::size_t i = 0; i < list1.size(); ++i) {
        total_distance += std::abs(list1[i] - list2[i]);
    }

    return total_distance;
}

auto solve_b() -> int64_t {
    std::pair<std::vector<int>, std::vector<int>> input_data = parse_input();
    std::vector<int> list1 = input_data.first;
    std::vector<int> list2 = input_data.second;

    std::unordered_map<int, int> list2_counts;
    for(const int value : list2) {
        list2_counts[value]++;
    }

    int64_t similarity_score = 0;
    for(const int value : list1) {
        similarity_score += value * list2_counts[value];
    }

    return similarity_score;
}

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