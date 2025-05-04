#include <cstdint> // std::size_t, int64_t
#include <fstream> // std::ifstream
#include <string>  // std::getline, std::stoi
#include <vector>

#include "day.hpp"

namespace Day22 {

const int64_t PRUNE_MOD = 16777216;
const int64_t MULTIPLY1_SCALAR = 64;
const int64_t DIVIDE_SCALAR = 32;
const int64_t MULTIPLY2_SCALAR = 2048;

auto parse_input() -> std::vector<int64_t> {
    const std::string input_file_name = "data/day22.txt";
    std::ifstream input_file{input_file_name};

    std::vector<int64_t> secret_numbers;
    std::string line;
    while(std::getline(input_file, line)) {
        secret_numbers.push_back(std::stoi(line));
    }

    return secret_numbers;
}

auto mix_and_prune(const int64_t previous_secret_number, const int64_t num)
    -> int64_t {
    // NOLINTNEXTLINE(hicpp-signed-bitwise)
    return (previous_secret_number ^ num) % PRUNE_MOD;
}

auto calculate_next_secret_number(const int64_t secret_number) -> int64_t {
    int64_t result =
        mix_and_prune(secret_number, MULTIPLY1_SCALAR * secret_number);
    result = mix_and_prune(result, result / DIVIDE_SCALAR);
    result = mix_and_prune(result, MULTIPLY2_SCALAR * result);
    return result;
}

} // namespace Day22

auto solve_day22a() -> int64_t {
    const std::vector<int64_t> initial_secret_numbers = Day22::parse_input();
    const int num_updates = 2000;
    int64_t total = 0;
    for(const int64_t initial_secret_number : initial_secret_numbers) {
        int64_t secret_number = initial_secret_number;
        for(int i = 0; i < num_updates; ++i) {
            secret_number = Day22::calculate_next_secret_number(secret_number);
        }
        total += secret_number;
    }

    return total;
}

auto solve_day22b() -> int64_t { return 0; }