#include "unixpath.h"

UnixPath::UnixPath(std::string_view initial_dir) {
    ChangeDirectory(initial_dir);
    this->initial_dir_ = current_dir_;
}

void UnixPath::ChangeDirectory(std::string_view path) {
    if (!path.empty() && path[0] == '/') {
        current_dir_.clear();
    }
    size_t lb = 0, rb = 0;
    while (rb <= path.size()) {
        if (path[rb] == '/' || rb == path.size()) {
            if (lb != rb) {
                if (path[lb] == '.') {
                    if (rb - lb == 2 && !current_dir_.empty()) {
                        current_dir_.pop_back();
                    }
                } else {
                    current_dir_.push_back(path.substr(lb, rb - lb));
                }
            }
            lb = rb = rb + 1;
        } else {
            rb++;
        }
    }
}

std::string UnixPath::GetAbsolutePath() const {
    std::string res;
    for (const auto& i : current_dir_) {
        res += "/";
        res += i;
    }
    return res.empty() ? "/" : res;
}

std::string UnixPath::GetRelativePath() const {
    auto it1 = initial_dir_.begin();
    auto it2 = current_dir_.begin();
    while (it1 != initial_dir_.end() && it2 != current_dir_.end() && *it1 == *it2) {
        it1++;
        it2++;
    };

    std::string result;
    for (size_t i = 0; i < static_cast<size_t>(initial_dir_.end() - it1); ++i) {
        result += "../";
    }

    if (result.empty()) {
        result = ".";
    } else {
        result.pop_back();
    }

    while (it2 != current_dir_.end()) {
        result += "/";
        result += *it2;
        it2++;
    }

    return result;
}
