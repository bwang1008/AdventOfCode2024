#include <algorithm> // std::set_intersection, std::sort
#include <cstdint>   // std::size_t, int64_t
#include <fstream>   // std::ifstream
#include <iostream>  // std::cout, std::endl
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

auto grow_tightly_coupled_computers(
    const std::vector<std::string> &current_tightly_coupled_computers,
    const std::map<std::string, std::set<std::string>> &edges)
    -> std::vector<std::vector<std::string>> {
    std::vector<std::vector<std::string>> next_tightly_coupled_computers;
    std::set<std::string> common_neighbors =
        edges.at(current_tightly_coupled_computers[0]);
    for(const std::string &current_computer :
        current_tightly_coupled_computers) {
        const std::set<std::string> &current_computer_neighbors =
            edges.at(current_computer);
        std::set<std::string> intersection;
        std::set_intersection(
            common_neighbors.begin(), common_neighbors.end(),
            current_computer_neighbors.begin(),
            current_computer_neighbors.end(),
            std::inserter(intersection, intersection.begin()));
        common_neighbors = intersection;
    }

    for(const std::string &common_neighbor : common_neighbors) {
        std::vector<std::string> next_tightly_coupled_computer =
            current_tightly_coupled_computers;
        next_tightly_coupled_computer.push_back(common_neighbor);
        std::sort(next_tightly_coupled_computer.begin(),
                  next_tightly_coupled_computer.end());
        next_tightly_coupled_computers.push_back(next_tightly_coupled_computer);
    }

    return next_tightly_coupled_computers;
}

auto find_biggest_tightly_coupled_computers(
    const std::vector<std::pair<std::string, std::string>> &connections)
    -> std::set<std::vector<std::string>> {
    const std::map<std::string, std::set<std::string>> edges =
        generate_edges(connections);

    std::set<std::vector<std::string>> current_tightly_coupled_computers;
    for(const std::pair<std::string, std::string> &connection : connections) {
        current_tightly_coupled_computers.insert(
            std::vector<std::string>{connection.first, connection.second});
    }
    std::set<std::vector<std::string>> next_tightly_coupled_computers;
    bool has_next = true;

    while(has_next) {
        for(const std::vector<std::string> &tightly_coupled_computers :
            current_tightly_coupled_computers) {
            const std::vector<std::vector<std::string>> next =
                grow_tightly_coupled_computers(tightly_coupled_computers,
                                               edges);
            next_tightly_coupled_computers.insert(next.begin(), next.end());
        }
        has_next = !next_tightly_coupled_computers.empty();

        if(has_next) {
            current_tightly_coupled_computers = next_tightly_coupled_computers;
            next_tightly_coupled_computers.clear();
            std::cout << "Now there are "
                      << current_tightly_coupled_computers.size() << " parties"
                      << std::endl;
        }
    }

    return current_tightly_coupled_computers;
}

auto get_password(std::vector<std::string> tightly_coupled_computers)
    -> std::string {
    std::sort(tightly_coupled_computers.begin(),
              tightly_coupled_computers.end());
    std::string password;
    for(std::size_t i = 0; i < tightly_coupled_computers.size(); ++i) {
        password += tightly_coupled_computers[i];
        if(i + 1 < tightly_coupled_computers.size()) {
            password += ',';
        }
    }

    return password;
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

auto solve_day23b() -> int64_t {
    const std::vector<std::pair<std::string, std::string>> connections =
        Day23::parse_input();
    const std::set<std::vector<std::string>> biggest_tightly_coupled_computers =
        Day23::find_biggest_tightly_coupled_computers(connections);

    for(const std::vector<std::string> &tightly_coupled_computers :
        biggest_tightly_coupled_computers) {
        std::cout << Day23::get_password(tightly_coupled_computers)
                  << std::endl;
    }
    return 0;
}