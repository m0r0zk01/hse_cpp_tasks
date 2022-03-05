#include "unixpath.h"
#include <deque>

void FillPathDeque(std::deque<std::string_view>& path_deque, std::string_view path) {
    size_t lb = 0, rb = 0;
    while (rb <= path.size()) {
        if (path[rb] == '/' || rb == path.size()) {
            if (lb != rb) {
                if (path[lb] == '.') {
                    if (rb - lb == 2 && !path_deque.empty()) {
                        path_deque.pop_back();
                    }
                } else {
                    path_deque.push_back(path.substr(lb, rb - lb));
                }
            }
            lb = rb = rb + 1;
        } else {
            rb++;
        }
    }
}

std::string GetResult(std::deque<std::string_view>& path_deque) {
    std::string result;
    while (!path_deque.empty()) {
        result += "/";
        result += path_deque.front();
        path_deque.pop_front();
    }
    return result;
}

std::string NormalizePath(std::string_view current_working_dir, std::string_view path) {
    std::deque<std::string_view> path_deque;
    if (!path.empty() && path[0] != '/') {
        FillPathDeque(path_deque, current_working_dir);
    }
    FillPathDeque(path_deque, path);

    return path_deque.empty() ? "/" : GetResult(path_deque);
}
