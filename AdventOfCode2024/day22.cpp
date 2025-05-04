#include <algorithm> // std::max
#include <cstdint>   // std::size_t, int64_t
#include <fstream>   // std::ifstream
#include <map>
#include <set>
#include <string> // std::getline, std::stoi
#include <tuple>  // std::get
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

auto generate_prices(const int64_t initial_secret_number) -> std::vector<int> {
    const int num_updates = 2000;
    const int64_t base_system = 10;
    std::vector<int> prices{
        static_cast<int>(initial_secret_number % base_system)};
    int64_t secret_number = initial_secret_number;
    for(int i = 0; i < num_updates; ++i) {
        secret_number = Day22::calculate_next_secret_number(secret_number);
        prices.push_back(static_cast<int>(secret_number % base_system));
    }

    return prices;
}

auto calculate_best_num_bananas(
    const std::vector<int64_t> &initial_secret_numbers) -> int64_t {
    std::vector<std::vector<int>> prices_of_vendors(
        initial_secret_numbers.size());
    for(std::size_t i = 0; i < prices_of_vendors.size(); ++i) {
        prices_of_vendors[i] = generate_prices(initial_secret_numbers[i]);
    }

    const std::size_t sequence_length = 4;
    std::map<std::tuple<int, int, int, int>, int> sequence_to_total_bananas;
    for(const std::vector<int> &prices : prices_of_vendors) {
        std::set<std::tuple<int, int, int, int>> seen_sequences;
        for(std::size_t i = 0; i + sequence_length < prices.size(); ++i) {
            const int num_bananas_at_sell = prices[i + sequence_length];
            const std::tuple<int, int, int, int> sequence(
                prices[i + 1] - prices[i], prices[i + 2] - prices[i + 1],
                prices[i + 3] - prices[i + 2], prices[i + 4] - prices[i + 3]);
            if(seen_sequences.find(sequence) == seen_sequences.end()) {
                seen_sequences.insert(sequence);
                sequence_to_total_bananas[sequence] += num_bananas_at_sell;
            }
        }
    }

    int best_number_of_bananas = 0;
    for(const auto &seq : sequence_to_total_bananas) {
        best_number_of_bananas = std::max(seq.second, best_number_of_bananas);
    }

    return best_number_of_bananas;
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

auto solve_day22b() -> int64_t {
    const std::vector<int64_t> initial_secret_numbers = Day22::parse_input();
    return Day22::calculate_best_num_bananas(initial_secret_numbers);
}