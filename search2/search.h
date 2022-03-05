#pragma once

#include <cctype>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <unordered_set>

std::string GetLowercase(const std::string_view s);

struct CaseInsensitiveStringHash {
    size_t operator()(const std::string_view obj) const;
};

struct CaseInsensitiveStringComparator {
    bool operator()(const std::string_view a, const std::string_view b) const;
};

class SearchEngine {
public:
    struct WordInfo {
        std::unordered_map<size_t, size_t> amount_in_line;
        size_t lines_count = 0;
    };

    struct DocumentInfo {
        std::string_view text;
    };

    size_t total_lines;
    std::vector<std::string_view> lines;

    std::vector<std::string_view> Split(const std::string_view text, bool by_lines) const;

    void BuildIndex(std::string_view text);
    std::vector<std::string_view> Search(std::string_view query, size_t results_count) const;

private:
    std::unordered_map<std::string_view, WordInfo, CaseInsensitiveStringHash, CaseInsensitiveStringComparator>
        word_info_;
    std::vector<size_t> line_word_count_;
};
