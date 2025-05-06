#include <algorithm> // std::sort
#include <cstdint>   // std::size_t, int64_t
#include <fstream>   // std::ifstream
#include <map>
#include <string>  // std::getline
#include <tuple>   // std::get
#include <utility> // std::pair
#include <vector>

#include "day.hpp"
#include "utils.hpp" // split()

namespace Day24 {

auto parse_input() -> std::pair<
    std::map<std::string, bool>,
    std::map<std::string, std::tuple<std::string, std::string, std::string>>> {
    const std::string input_file_name = "data/day24.txt";
    std::ifstream input_file{input_file_name};

    std::map<std::string, bool> initial_values;
    std::map<std::string, std::tuple<std::string, std::string, std::string>>
        gates;

    std::string line;
    bool first_part = true;
    while(std::getline(input_file, line)) {
        if(line.empty()) {
            first_part = false;
            continue;
        }
        if(first_part) {
            const std::string name = line.substr(0, 3);
            const bool value = (line.substr(line.size() - 1, 1) == "1");
            initial_values[name] = value;
        } else {
            const std::vector<std::string> parts = split(line, ' ');
            gates[parts[4]] = std::tuple<std::string, std::string, std::string>(
                parts[0], parts[1], parts[2]);
        }
    }

    return std::pair<std::map<std::string, bool>,
                     std::map<std::string, std::tuple<std::string, std::string,
                                                      std::string>>>(
        initial_values, gates);
}

auto compute_all_values(
    const std::map<std::string, bool> &initial_values,
    const std::map<std::string,
                   std::tuple<std::string, std::string, std::string>> &gates)
    -> std::map<std::string, bool> {
    std::map<std::string, bool> known_values(initial_values.begin(),
                                             initial_values.end());

    std::vector<std::string> stack(gates.size());
    std::size_t index = 0;
    for(const auto &gate : gates) {
        stack[index] = gate.first;
        ++index;
    }

    while(!stack.empty()) {
        const std::string curr = stack.back();
        if(known_values.find(curr) != known_values.end()) {
            stack.pop_back();
            continue;
        }
        bool children_known = true;
        const std::string &child1 = std::get<0>(gates.at(curr));
        const std::string &op_name = std::get<1>(gates.at(curr));
        const std::string &child2 = std::get<2>(gates.at(curr));
        if(known_values.find(child1) == known_values.end()) {
            children_known = false;
            stack.push_back(child1);
        }
        if(known_values.find(child2) == known_values.end()) {
            children_known = false;
            stack.push_back(child2);
        }
        if(!children_known) {
            continue;
        }

        const bool operand1 = known_values.at(child1);
        const bool operand2 = known_values.at(child2);
        bool result = false;
        if(op_name == "AND") {
            result = operand1 && operand2;
        } else if(op_name == "OR") {
            result = operand1 || operand2;
        } else if(op_name == "XOR") {
            result = operand1 ^ operand2;
        }
        known_values[curr] = result;
        stack.pop_back();
    }

    return known_values;
}

auto compute_number(
    const std::map<std::string, bool> &initial_values,
    const std::map<std::string,
                   std::tuple<std::string, std::string, std::string>> &gates)
    -> int64_t {
    const std::map<std::string, bool> known_values =
        compute_all_values(initial_values, gates);
    std::vector<std::pair<std::string, bool>> z_values;
    for(const auto &known_value : known_values) {
        const std::string gate_name = known_value.first;
        const bool value = known_value.second;
        if(gate_name[0] == 'z') {
            z_values.emplace_back(gate_name, value);
        }
    }

    std::sort(z_values.begin(), z_values.end());

    int64_t num = 0;
    for(auto it = z_values.rbegin(); it != z_values.rend(); ++it) {
        num = 2 * num + ((it->second) ? 1 : 0);
    }

    return num;
}

} // namespace Day24

auto solve_day24a() -> int64_t {
    const std::pair<std::map<std::string, bool>,
                    std::map<std::string,
                             std::tuple<std::string, std::string, std::string>>>
        inputs = Day24::parse_input();
    const std::map<std::string, bool> initial_values = inputs.first;
    const std::map<std::string,
                   std::tuple<std::string, std::string, std::string>>
        gates = inputs.second;
    return Day24::compute_number(initial_values, gates);
}

auto solve_day24b() -> int64_t { return 0; }