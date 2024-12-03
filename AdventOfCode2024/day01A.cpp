#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

auto main() -> int {
    const std::string input_file_name{"data/day01.txt"};
    std::ifstream input_file{input_file_name};
    if(!input_file) {
        std::cerr << "Could not open file " << input_file_name << std::endl;
        return 1;
    }

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

    std::sort(list1.begin(), list1.end());
    std::sort(list2.begin(), list2.end());

    int total_distance = 0;
    for(std::size_t i = 0; i < list1.size(); ++i) {
        total_distance += std::abs(list1[i] - list2[i]);
    }

    std::cout << total_distance << std::endl;
    return 0;
}
