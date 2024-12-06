#include <algorithm>
#include <cstddef>
#include <fstream>
#include <map>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "day.hpp"
#include "utils.hpp"

static auto parse_input() -> std::pair<std::map<int, std::unordered_set<int>>,
                                       std::vector<std::vector<int>>> {
    const std::string input_file_name{"data/day05.txt"};
    std::ifstream input_file{input_file_name};

    std::map<int, std::unordered_set<int>> edges;
    std::vector<std::vector<int>> manual_pages;

    std::string line;
    bool first_part = true;
    while(std::getline(input_file, line)) {
        if(line.empty()) {
            first_part = false;
            continue;
        }

        if(first_part) {
            const std::vector<std::string> line_parts = split(line, '|');

            const int num1 = std::stoi(line_parts[0]);
            const int num2 = std::stoi(line_parts[line_parts.size() - 1]);
            edges[num1].insert(num2);
        } else {
            const std::vector<std::string> line_parts = split(line, ',');
            std::vector<int> pages(line_parts.size());
            std::transform(
                line_parts.cbegin(), line_parts.cend(), pages.begin(),
                [](const std::string &s) -> int { return std::stoi(s); });
            manual_pages.push_back(pages);
        }
    }

    return std::pair(edges, manual_pages);
}

auto is_valid_manual(const std::map<int, std::unordered_set<int>> &edges,
                     const std::vector<int> &pages) -> bool {
    for(std::size_t j = 0; j < pages.size(); ++j) {
        for(std::size_t i = 0; i < j; ++i) {
            const int prev_page = pages[i];
            const int curr_page = pages[j];
            // search if this violates a rule
            if(edges.find(curr_page) != edges.end() &&
               edges.at(curr_page).find(prev_page) !=
                   edges.at(curr_page).end()) {
                return false;
            }
        }
    }

    return true;
}

auto topological_sort(const std::map<int, std::unordered_set<int>> &edges,
                      const std::vector<int> &arr) -> std::vector<int> {
    const std::unordered_set<int> arr_set(arr.begin(), arr.end());

    std::vector<int> stack(arr.begin(), arr.end());
    std::vector<int> processed;
    std::unordered_set<int> have_added_children;
    std::unordered_set<int> have_added_to_processed;
    while(!stack.empty()) {
        const int last = stack.back();

        if(have_added_to_processed.find(last) !=
           have_added_to_processed.end()) {
            stack.pop_back();
            continue;
        }

        if(have_added_children.find(last) == have_added_children.end()) {
            // first time seeing this element, so expand
            have_added_children.insert(last);
            if(edges.find(last) == edges.end()) {
                continue;
            }
            for(const int child : edges.at(last)) {
                if(arr_set.find(child) != arr_set.end() &&
                   have_added_children.find(child) ==
                       have_added_children.end()) {
                    stack.push_back(child);
                }
            }

        } else {
            // second time seeing this element
            processed.push_back(last);
            have_added_to_processed.insert(last);
            stack.pop_back();
        }
    }

    std::reverse(processed.begin(), processed.end());
    return processed;
}

auto solve_day05a() -> int64_t {
    std::pair<std::map<int, std::unordered_set<int>>,
              std::vector<std::vector<int>>>
        input = parse_input();

    const std::map<int, std::unordered_set<int>> edges = input.first;
    const std::vector<std::vector<int>> manual_pages = input.second;

    int64_t sum_middle = 0;
    for(const std::vector<int> &pages : manual_pages) {
        if(is_valid_manual(edges, pages)) {
            sum_middle += pages[pages.size() / 2];
        }
    }

    return sum_middle;
}

auto solve_day05b() -> int64_t {
    std::pair<std::map<int, std::unordered_set<int>>,
              std::vector<std::vector<int>>>
        input = parse_input();

    const std::map<int, std::unordered_set<int>> edges = input.first;
    const std::vector<std::vector<int>> manual_pages = input.second;

    int64_t sum_middle = 0;
    for(const std::vector<int> &pages : manual_pages) {
        if(!is_valid_manual(edges, pages)) {
            const std::vector<int> fixed_pages = topological_sort(edges, pages);
            sum_middle += fixed_pages[fixed_pages.size() / 2];
        }
    }

    return sum_middle;
}
