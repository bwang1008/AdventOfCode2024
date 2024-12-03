#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

auto main() -> int {
    const std::string input_file_name{"data/day01.txt"};
    std::ifstream input_file{input_file_name};
    if(!input_file) {
        std::cerr << "Could not open file " << input_file_name << std::endl;
        return 1;
    }

    std::vector<int> list1;
    std::unordered_map<int, int> list2_counts;
    bool first = true;
    int location_id = 0;

    while(input_file >> location_id) {
        if(first) {
            list1.push_back(location_id);
        } else {
            list2_counts[location_id]++;
        }
        first = !first;
    }

    int64_t similarity_score = 0;
    for(const int value : list1) {
        similarity_score += value * list2_counts[value];
    }

    std::cout << similarity_score << std::endl;
    return 0;
}
