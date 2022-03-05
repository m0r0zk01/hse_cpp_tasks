#pragma once

#include <deque>
#include <iostream>
#include <string>
#include <string_view>

class UnixPath {
public:
    UnixPath(std::string_view initial_dir);

    void ChangeDirectory(std::string_view path);

    std::string GetAbsolutePath() const;
    std::string GetRelativePath() const;

private:
    std::deque<std::string_view> initial_dir_;
    std::deque<std::string_view> current_dir_;
};
