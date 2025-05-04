#include <algorithm> // std::set_intersection
#include <cstdint>   // std::size_t, int64_t
#include <fstream>   // std::ifstream
#include <iterator>  // std::inserter
#include <map>
#include <set>
#include <string>  // std::getline
#include <utility> // std::pair
#include <vector>

#include "day.hpp"
#include "utils.hpp" // split()

namespace Day23 {

auto parse_input() -> std::vector<std::pair<std::string, std::string>> {
    const std::string input_file_name = "data/day23.txt";
    std::ifstream input_file{input_file_name};

    std::vector<std::pair<std::string, std::string>> results;
    std::string line;
    while(std::getline(input_file, line)) {
        const std::vector<std::string> parts = split(line, '-');
        results.emplace_back(parts[0], parts[1]);
    }

    return results;
}

auto generate_edges(
    const std::vector<std::pair<std::string, std::string>> &connections)
    -> std::map<std::string, std::set<std::string>> {
    std::map<std::string, std::set<std::string>> edges;
    for(const std::pair<std::string, std::string> &connection : connections) {
        edges[connection.first].insert(connection.second);
        edges[connection.second].insert(connection.first);
    }

    return edges;
}

auto find_all_triangles(
    const std::vector<std::pair<std::string, std::string>> &connections)
    -> std::vector<std::vector<std::string>> {
    std::vector<std::vector<std::string>> triangles;
    const std::map<std::string, std::set<std::string>> edges =
        generate_edges(connections);
    for(const auto &edge1 : edges) {
        const std::string first = edge1.first;
        const std::set<std::string> neighbors = edge1.second;
        for(const std::string &second : neighbors) {
            if(first >= second) {
                continue;
            }
            const std::set<std::string> &second_neighbors = edges.at(second);
            std::set<std::string> thirds;
            std::set_intersection(
                neighbors.begin(), neighbors.end(), second_neighbors.begin(),
                second_neighbors.end(), std::inserter(thirds, thirds.begin()));
            for(const std::string &third : thirds) {
                if(second >= third) {
                    continue;
                }
                triangles.push_back(
                    std::vector<std::string>{first, second, third});
            }
        }
    }

    return triangles;
}

} // namespace Day23

auto solve_day23a() -> int64_t {
    const std::vector<std::pair<std::string, std::string>> connections =
        Day23::parse_input();
    const std::vector<std::vector<std::string>> triangles =
        Day23::find_all_triangles(connections);
    int64_t count = 0;
    for(const std::vector<std::string> &triangle : triangles) {
        bool starts_with_t = false;
        for(const std::string &computer_name : triangle) {
            if(computer_name[0] == 't') {
                starts_with_t = true;
                break;
            }
        }
        if(starts_with_t) {
            ++count;
        }
    }
    return count;
}

auto solve_day23b() -> int64_t { return 0; }