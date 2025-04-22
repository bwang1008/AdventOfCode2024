#include <cstdint>   // std::size_t, int64_t
#include <fstream>   // std::ifstream
#include <iostream>  // std::cout, std::endl
#include <stdexcept> // std::invalid_argument
#include <string>    // std::getline
#include <tuple>     // std::get
#include <vector>

#include "day.hpp"

namespace Day17 {

static auto parse_input()
    -> std::tuple<std::vector<int>, int64_t, int64_t, int64_t> {
    const std::string input_file_name = "data/day17.txt";
    std::ifstream input_file{input_file_name};

    std::vector<std::vector<char>> board;
    std::pair<std::size_t, std::size_t> start_location(0, 0);
    std::pair<std::size_t, std::size_t> end_location(0, 0);

    const std::size_t num_registers = 3;
    std::vector<int64_t> register_values;
    for(std::size_t i = 0; i < num_registers; ++i) {
        std::string register_line;
        std::getline(input_file, register_line);
        const std::string prefix = "Register A: ";
        register_values.push_back(std::stoi(register_line.substr(
            prefix.size(), register_line.size() - prefix.size())));
    }

    std::string program_line;
    std::getline(input_file, program_line);
    std::getline(input_file, program_line);
    const std::string program_prefix = "Program: ";
    const std::string program_steps_line = program_line.substr(
        program_prefix.size(), program_line.size() - program_prefix.size());
    const char delimiter = ',';

    std::vector<int> program_steps;
    std::size_t prev = 0;
    std::size_t curr = 0;
    while((curr = program_steps_line.find(delimiter, prev)) !=
          std::string::npos) {
        program_steps.push_back(
            std::stoi(program_steps_line.substr(prev, curr - prev)));
        prev = curr + 1;
    }
    program_steps.push_back(std::stoi(
        program_steps_line.substr(prev, program_steps_line.size() - prev)));

    return std::tuple<std::vector<int>, int64_t, int64_t, int64_t>(
        program_steps, register_values[0], register_values[1],
        register_values[2]);
}

class Computer {
  public:
    Computer(const int64_t a, const int64_t b, const int64_t c)
        : instruction_pointer(0), modified_instruction_pointer(false),
          register_a(a), register_b(b), register_c(c) {
        this->instruction_table = {
            &Computer::instruction_adv, &Computer::instruction_bxl,
            &Computer::instruction_bst, &Computer::instruction_jnz,
            &Computer::instruction_bxc, &Computer::instruction_out,
            &Computer::instruction_bdv, &Computer::instruction_cdv};
    }

    [[nodiscard]] auto get_register(const char c) const -> int64_t {
        if(c == 'A') {
            return this->register_a;
        }
        if(c == 'B') {
            return this->register_b;
        }
        if(c == 'C') {
            return this->register_c;
        }

        throw std::invalid_argument("Register must be one of 'A', 'B', 'C'");
    }

    [[nodiscard]] auto get_output() const -> std::vector<int> {
        return this->output;
    }

    [[nodiscard]] auto interpret_operand(const int operand,
                                         const bool is_combo) const -> int64_t {
        if(!is_combo) {
            return operand;
        }
        if(0 <= operand && operand <= 3) {
            return operand;
        }

        const int register_a_operand = 4;
        const int register_b_operand = 5;
        const int register_c_operand = 6;
        const int register_illegal_operand = 7;

        if(operand == register_a_operand) {
            return this->register_a;
        }
        if(operand == register_b_operand) {
            return this->register_b;
        }
        if(operand == register_c_operand) {
            return this->register_c;
        }
        if(operand == register_illegal_operand) {
            throw std::invalid_argument(
                "Combo 7 is not allowed in a valid program");
        }

        throw std::invalid_argument("Combo " + std::to_string(operand) +
                                    " is not defined");
    }

    auto run_program(const std::vector<int> &program) -> void {
        this->instruction_pointer = 0;
        this->output.clear();

        while(this->instruction_pointer < program.size()) {
            this->modified_instruction_pointer = false;
            if(this->instruction_pointer + 1 >= program.size()) {
                throw std::invalid_argument(
                    "Error trying to retrieve operand outside of program");
            }
            const auto opcode =
                static_cast<std::size_t>(program[this->instruction_pointer]);
            const int operand = program[this->instruction_pointer + 1];
            (this->*instruction_table[opcode])(operand);

            if(!modified_instruction_pointer) {
                this->instruction_pointer += 2;
            }
        }
    }

  private:
    std::size_t instruction_pointer;
    bool modified_instruction_pointer;
    int64_t register_a;
    int64_t register_b;
    int64_t register_c;
    std::vector<int> output{};
    std::vector<void (Computer::*)(int)> instruction_table{};

    [[nodiscard]] auto compute_division(const int operand) const -> int64_t {
        int64_t numerator = this->register_a;
        const int64_t power = this->interpret_operand(operand, true);
        for(int64_t i = 0; i < power; ++i) {
            numerator /= 2;
        }
        return numerator;
    }

    auto instruction_adv(const int operand) -> void {
        this->register_a = this->compute_division(operand);
    }

    auto instruction_bxl(const int operand) -> void {
        this->register_b =
            // NOLINTNEXTLINE(hicpp-signed-bitwise)
            this->register_b ^ this->interpret_operand(operand, false);
    }

    auto instruction_bst(const int operand) -> void {
        const int64_t modulo = 8;
        this->register_b = this->interpret_operand(operand, true) % modulo;
    }

    auto instruction_jnz(const int operand) -> void {
        if(this->register_a == 0) {
            return;
        }
        this->instruction_pointer =
            static_cast<std::size_t>(this->interpret_operand(operand, false));
        this->modified_instruction_pointer = true;
    }

    auto instruction_bxc(const int /*operand*/) -> void {
        this->register_b ^= this->register_c; // NOLINT(hicpp-signed-bitwise)
    }

    auto instruction_out(const int operand) -> void {
        const int64_t modulo = 8;
        this->output.push_back(
            static_cast<int>(this->interpret_operand(operand, true) % modulo));
    }

    auto instruction_bdv(const int operand) -> void {
        this->register_b = this->compute_division(operand);
    }

    auto instruction_cdv(const int operand) -> void {
        this->register_c = this->compute_division(operand);
    }
};

} // namespace Day17

auto solve_day17a() -> int64_t {
    const std::tuple<std::vector<int>, int64_t, int64_t, int64_t> inputs =
        Day17::parse_input();
    const std::vector<int> &program = std::get<0>(inputs);
    const int64_t register_a = std::get<1>(inputs);
    const int64_t register_b = std::get<2>(inputs);
    const int64_t register_c = std::get<3>(inputs);

    Day17::Computer computer(register_a, register_b, register_c);
    computer.run_program(program);
    const std::vector<int> output = computer.get_output();

    constexpr char delimiter = ',';
    for(std::size_t i = 0; i < output.size(); ++i) {
        std::cout << output[i];
        if(i + 1 < output.size()) {
            std::cout << delimiter;
        }
    }
    std::cout << std::endl;

    return 0;
}

auto solve_day17b() -> int64_t {
    const std::tuple<std::vector<int>, int64_t, int64_t, int64_t> inputs =
        Day17::parse_input();
    const std::vector<int> &program = std::get<0>(inputs);
    const int64_t register_b = std::get<2>(inputs);
    const int64_t register_c = std::get<3>(inputs);

    bool found_identity = false;
    int64_t register_a = 0;

    while(!found_identity) {
        Day17::Computer computer(register_a, register_b, register_c);
        computer.run_program(program);
        const std::vector<int> output = computer.get_output();
        if(output == program) {
            found_identity = true;
        }
    }

    return register_a;
}
