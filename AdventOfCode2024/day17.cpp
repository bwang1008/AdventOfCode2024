/**
Interpretation of given input program:
2,4: B = A % 8
1,2: B ^= 2
7,5: C = A/(2 ^ B)
4,5: B ^= C
0,3: A = A/(2^3)
1,7: B ^= 7
5,5: print(B % 8)
3,0: jump to first instruction again, until A is 0

------------

B = (A % 8) ^ 2
B = B ^ (A / (2 ** B)) ^ 7
print(B % 8)
A /= 8
go to beginning

------------

B = 5 ^ (A % 8) ^ (A / (2 ** ((A % 8) ^ 2)))
print(B % 8)
A /= 8
go to beginning

-----------

So to get output of 2...
if A % 8 == 7, then B = (2 ^ (A / 32)) which mod 8 is 2
So (A/32) % 8 must be 0.

if A % 8 == 6, then B = (3 ^ (A / 16)) which mod 8 is 2.
So (A / 16) % 8 must be 1.

if A % 8 == 5, then B = (A / 128) which mod 8 is 2.
So (A / 128) % 8 must be 2.

At the very least, can think about generating the digits backwards and build
out register_a initial value from it.

answer = 190384615275535
How I generated my answer: had code search for what created the last 4 numbers: [5, 5, 3, 0]
For instance, A = 2770.
Then I checked which of [8A, 8A + 1, ..., 8A + 7] generated [7, 5, 5, 3, 0].
I got A2 = 8A + 3 == 22163.
Then I checked which of [8A2, 8A2 + 1, ..., 8A2 + 7] generated [1,7,5,5,3,0]
and so on.
*/

#include <cstdint>   // std::size_t, int64_t
#include <fstream>   // std::ifstream
#include <iostream>  // std::cout, std::endl
#include <stdexcept> // std::invalid_argument
#include <string>    // std::getline
#include <tuple>     // std::get
#include <vector>

#include "day.hpp"

namespace Day17 {

auto parse_input()
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
    Computer(const int64_t a, const int64_t b, const int64_t c, const bool short_circuit)
        : instruction_pointer(0), modified_instruction_pointer(false),
          register_a(a), register_b(b), register_c(c), register_a_initial(a), short_circuit_identity(short_circuit) {
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

            if(this->short_circuit_identity && !this->output.empty()) {
                if(this->output.size() > program.size()) {
                    return;
                }
                const std::size_t last = this->output.size() - 1;
                if(last >= 8) {
                    std::cout << "Almost: last = " << last << " and a_value = " << this->register_a_initial << std::endl;
                }
                if(this->output[last] != program[last]) {
                    return;
                }
            }
        }
    }

  private:
    std::size_t instruction_pointer;
    bool modified_instruction_pointer;
    int64_t register_a;
    int64_t register_b;
    int64_t register_c;
    int64_t register_a_initial;
    bool short_circuit_identity;
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


auto run_program(int64_t register_a) -> std::vector<int> {
    std::vector<int> output;
    
    while(register_a != 0) {
        int64_t register_b = (register_a % 8) ^ 2;
        int64_t register_c = register_a;
        for(int i = 0; i < register_b; ++i) {
            register_c /= 2;
        }
        register_b ^= register_c ^ 7;
        output.push_back(static_cast<int>(register_b % 8));
        register_a /= 8;
    }

    return output;
}

/** 
 * Return initial value of register_a such that run_program(register_a) results in 
 * desired_output. Return -1 if not possible.
 * 
 * end of desired_output corresponds to most significant bits of register_a.
 * So build up digits of register_a, starting from most significant down to least significant.
 * We can do this because each digit printed by program is mainly decided by last 3 bits.
 * 
 * For instance, to build up [5, 5, 3, 0],
 * Try A = 0, A = 1, ..., A = 7 such that run_program(A) results in [0].
 * In this case, A = 5.
 * Then try A = 8 * 5 + 0, A = 8 * 5 + 1, ..., 8 * 5 + 7 such that run_program(A) results in [3, 0]. 
 * In this case, A = 8 * 5 + 3 = 43.
 * Then try A = 8 * 43 + 0, A = 8 * 43 + 1, ..., 8 * 43 + 7 such that run_program(A) results in [5, 3, 0]
 * and so on.
 */
auto get_desired_output(const std::vector<int> &desired_output) -> int64_t {
    int64_t answer = 0;
    const int division_of_a = 8;
    for(std::vector<int>::const_reverse_iterator it = desired_output.crbegin(); it != desired_output.crend(); ++it) {
        const int desired_digit = *it;
        bool found_desired_digit = false;
        for(int i = 0; i < division_of_a; ++i) {
            const int64_t candidate = division_of_a * answer + i;
            const std::vector<int> output = run_program(candidate);
            if(!output.empty() && output[0] == desired_digit) {
                answer = candidate;
                found_desired_digit = true;
                break;
            }
        }
        if(!found_desired_digit) {
            return -1;
        }
    }

    return answer;
}

} // namespace Day17

auto solve_day17a() -> int64_t {
    const std::tuple<std::vector<int>, int64_t, int64_t, int64_t> inputs =
        Day17::parse_input();
    const std::vector<int> &program = std::get<0>(inputs);
    const int64_t register_a = std::get<1>(inputs);
    const int64_t register_b = std::get<2>(inputs);
    const int64_t register_c = std::get<3>(inputs);

    Day17::Computer computer(register_a, register_b, register_c, false);
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
    return Day17::get_desired_output(program);
}
