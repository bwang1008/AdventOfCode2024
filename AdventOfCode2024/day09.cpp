#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "day.hpp"

static auto parse_input() -> std::string {
    const std::string input_file_name{"data/day09.txt"};
    std::ifstream input_file{input_file_name};
    std::string input;
    std::getline(input_file, input);
    return input;
}

auto print_file_system(const std::vector<int> &file_system) -> void {
    for(const int i : file_system) {
        if(i == -1) {
            std::cout << '.';
        } else {
            std::cout << i;
        }
    }
    std::cout << std::endl;
}

auto expand_dense_memory_layout(const std::string &dense_memory_layout)
    -> std::vector<int> {
    const int max_digit = 9;
    std::vector<int> file_system(dense_memory_layout.size() * max_digit, -1);
    std::size_t file_system_index = 0;
    int file_id = 0;
    bool memory_used_by_file = true;
    for(const char c : dense_memory_layout) {
        const int digit = c - '0';
        if(memory_used_by_file) {
            for(int i = 0; i < digit; ++i) {
                file_system[file_system_index] = file_id;
                ++file_system_index;
            }
            ++file_id;
        } else {
            file_system_index += static_cast<std::size_t>(digit);
        }

        memory_used_by_file = !memory_used_by_file;
    }

    return file_system;
}

auto compact_files(std::vector<int> &file_system) -> void {
    std::size_t left = 0;
    std::size_t right = file_system.size() - 1;

    while(left < right) {
        while(left < right && file_system[left] != -1) {
            file_system[left] = file_system[left];
            ++left;
        }
        while(left < right && file_system[right] == -1) {
            file_system[right] = file_system[right];
            --right;
        }
        if(left < right) {
            file_system[left] = file_system[right];
            file_system[right] = -1;
        }
    }
}

auto checksum(const std::vector<int> &file_system) -> int64_t {
    int64_t sum = 0;
    for(std::size_t i = 0; i < file_system.size(); ++i) {
        if(file_system[i] != -1) {
            sum += static_cast<int>(i) * file_system[i];
        }
    }
    return sum;
}

auto solve_day09a() -> int64_t {
    const std::string dense_memory_layout = parse_input();
    std::vector<int> file_system =
        expand_dense_memory_layout(dense_memory_layout);
    compact_files(file_system);
    return checksum(file_system);
}

auto solve_day09b() -> int64_t {
    const int64_t result = 0;
    return result;
}