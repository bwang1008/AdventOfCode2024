#include <algorithm>
#include <cstdint>
#include <fstream>
#include <limits>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "day.hpp"

constexpr const int BASE = 10;

// https://codeforces.com/blog/entry/18051
class SegmentTree {
  public:
    explicit SegmentTree(const std::vector<std::size_t> &a)
        : N{a.size()}, arr(2 * a.size()) {
        for(std::size_t i = 0; i < N; ++i) {
            arr[i + N] = a[i];
        }
        for(std::size_t i = N - 1; i > 0; --i) {
            arr[i] = std::min(arr[2 * i], arr[2 * i + 1]);
        }
    }

    auto modify(std::size_t index, std::size_t value) -> void {
        for(arr[index += N] = value; index > 1; index /= 2) {
            const std::size_t companion_index =
                (index % 2 == 1) ? index - 1 : index + 1;
            arr[index / 2] = std::min(arr[index], arr[companion_index]);
        }
    }

    auto query(std::size_t left, std::size_t right) -> std::size_t {
        std::size_t result = std::numeric_limits<std::size_t>::max();
        for(left += N, right += N; left < right; left /= 2, right /= 2) {
            if(left % 2 == 1) {
                result = std::min(result, arr[left++]);
            }
            if(right % 2 == 1) {
                result = std::min(result, arr[--right]);
            }
        }

        return result;
    }

  private:
    std::size_t N;
    std::vector<std::size_t> arr;
};

static auto parse_input() -> std::string {
    const std::string input_file_name{"data/day09.txt"};
    std::ifstream input_file{input_file_name};
    std::string input;
    std::getline(input_file, input);
    return input;
}

auto expand_dense_memory_layout(const std::string &dense_memory_layout)
    -> std::vector<int> {
    const int max_digit = BASE - 1;
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

auto compact_file_system(std::vector<int> &file_system) -> void {
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

auto compute_checksum(const std::vector<int> &file_system) -> int64_t {
    int64_t checksum = 0;
    for(std::size_t i = 0; i < file_system.size(); ++i) {
        if(file_system[i] != -1) {
            checksum += static_cast<int>(i) * file_system[i];
        }
    }
    return checksum;
}

auto solve_day09a() -> int64_t {
    const std::string dense_memory_layout = parse_input();
    std::vector<int> file_system =
        expand_dense_memory_layout(dense_memory_layout);
    compact_file_system(file_system);
    return compute_checksum(file_system);
}

auto separate_out_existing_files_and_free_space(
    const std::string &dense_memory_layout)
    -> std::tuple<std::vector<std::pair<int, std::size_t>>,
                  std::map<int, std::set<std::size_t>>,
                  std::map<std::size_t, int>> {
    // index of vector is file_id; pair of (file size, which index in
    // file_system)
    std::vector<std::pair<int, std::size_t>> existing_files;

    // map from (size of contiguous free location) to indices where this
    // contiguous free location starts
    std::map<int, std::set<std::size_t>> free_locations;

    // map from (index of contiguous free location) to size of contiguous free
    // location
    std::map<std::size_t, int> index_to_free_size;

    std::size_t file_system_index = 0;
    bool memory_used_by_file = true;
    for(const char c : dense_memory_layout) {
        const int digit = c - '0';
        if(memory_used_by_file) {
            existing_files.emplace_back(digit, file_system_index);
        } else {
            const int size_of_contiguous_free_space = digit;
            free_locations[size_of_contiguous_free_space].insert(
                file_system_index);
            index_to_free_size[file_system_index] =
                size_of_contiguous_free_space;
        }

        file_system_index += static_cast<std::size_t>(digit);
        memory_used_by_file = !memory_used_by_file;
    }

    return std::tuple(existing_files, free_locations, index_to_free_size);
}

auto compact_by_files(std::vector<std::pair<int, std::size_t>> &existing_files,
                      std::map<int, std::set<std::size_t>> &free_space,
                      std::map<std::size_t, int> &index_to_free_size) -> void {
    std::vector<std::size_t> min_indices(BASE);
    for(int i = 0; i < BASE; ++i) {
        if(free_space.find(i) != free_space.end()) {
            min_indices[static_cast<std::size_t>(i)] =
                *free_space[static_cast<std::size_t>(i)].begin();
        } else {
            min_indices[static_cast<std::size_t>(i)] =
                std::numeric_limits<std::size_t>::max();
        }
    }
    SegmentTree st(min_indices);

    for(auto it = existing_files.rbegin(); it != existing_files.rend(); ++it) {
        const int file_size = it->first;
        const std::size_t left_most_available_index =
            st.query(static_cast<std::size_t>(file_size), BASE);

        if(left_most_available_index ==
           std::numeric_limits<std::size_t>::max()) {
            continue;
        }
        if(left_most_available_index > it->second) {
            continue;
        }

        const int available_space_size =
            index_to_free_size[left_most_available_index];
        // use up the space
        it->second = left_most_available_index;
        free_space[available_space_size].erase(
            free_space[available_space_size].begin());
        if(free_space[available_space_size].empty()) {
            st.modify(static_cast<std::size_t>(available_space_size),
                      std::numeric_limits<std::size_t>::max());
        } else {
            st.modify(static_cast<std::size_t>(available_space_size),
                      *free_space[available_space_size].begin());
        }
        index_to_free_size[left_most_available_index] = 0;

        const int remaining_space = available_space_size - file_size;
        if(remaining_space != 0) {
            free_space[remaining_space].insert(
                left_most_available_index +
                static_cast<std::size_t>(file_size));
            st.modify(static_cast<std::size_t>(remaining_space),
                      *free_space[remaining_space].begin());
            index_to_free_size[left_most_available_index +
                               static_cast<std::size_t>(file_size)] =
                remaining_space;
        }
    }
}

auto compute_checksum_given_file_size_and_index(
    const std::vector<std::pair<int, std::size_t>> &existing_files) -> int64_t {
    int64_t checksum = 0;
    for(std::size_t file_id = 0; file_id < existing_files.size(); ++file_id) {
        const int file_size = existing_files[file_id].first;
        const std::size_t file_system_index = existing_files[file_id].second;
        for(int file_system_index_offset = 0;
            file_system_index_offset < file_size; ++file_system_index_offset) {
            checksum += static_cast<int64_t>(file_id) *
                        (static_cast<int64_t>(file_system_index) +
                         file_system_index_offset);
        }
    }
    return checksum;
}

auto solve_day09b() -> int64_t {
    const std::string dense_memory_layout = parse_input();
    const std::tuple<std::vector<std::pair<int, std::size_t>>,
                     std::map<int, std::set<std::size_t>>,
                     std::map<std::size_t, int>>
        existing_files_and_free_space =
            separate_out_existing_files_and_free_space(dense_memory_layout);
    std::vector<std::pair<int, std::size_t>> existing_files =
        std::get<0>(existing_files_and_free_space);
    std::map<int, std::set<std::size_t>> free_space =
        std::get<1>(existing_files_and_free_space);
    std::map<std::size_t, int> index_to_free_size =
        std::get<2>(existing_files_and_free_space);
    compact_by_files(existing_files, free_space, index_to_free_size);
    return compute_checksum_given_file_size_and_index(existing_files);
}