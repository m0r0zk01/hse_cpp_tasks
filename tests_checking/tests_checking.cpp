#include "tests_checking.h"
#include <algorithm>
#include <deque>

std::vector<std::string> StudentsOrder(const std::vector<StudentAction>& student_actions,
                                       const std::vector<size_t>& queries) {
    std::deque<const std::string*> d;
    for (const auto& action : student_actions) {
        switch (action.side) {
            case Side::Bottom:
                d.push_front(&(action.name));
                break;
            case Side::Top:
                d.push_back(&(action.name));
                break;
            default:
                break;
        }
    }

    std::vector<size_t> sorted_queries = queries;
    sort(sorted_queries.begin(), sorted_queries.end());

    std::vector<std::string> result;
    size_t current_num = 1;
    auto it = sorted_queries.begin();
    while (it != sorted_queries.end()) {
        while (it != sorted_queries.end() && current_num == *it) {
            result.push_back(*(d.back()));
            ++it;
        }
        d.pop_back();
        current_num++;
    }

    return result;
}
