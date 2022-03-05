#include "search.h"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <iostream>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

std::string GetLowercase(const std::string_view s) {
    std::string lowercase_string;
    for (const auto c : s) {
        lowercase_string.push_back(std::tolower(c));
    }
    return lowercase_string;
}

struct CaseInsensitiveStringHash {
    size_t operator()(const std::string_view obj) const {
        std::hash<std::string> hash;
        return hash(GetLowercase(obj));
    }
};

struct CaseInsensitiveStringComparator {
    bool operator()(const std::string_view a, const std::string_view b) const {
        if (a.size() != b.size()) {
            return false;
        }

        std::string lower_a = GetLowercase(a);
        std::string lower_b = GetLowercase(b);

        for (size_t i = 0; i < lower_a.size(); ++i) {
            if (lower_a[i] != lower_b[i]) {
                return false;
            }
        }
        return true;
    }
};

struct WordInfo {
    std::unordered_map<size_t, size_t> amount_in_line;
    size_t lines_count = 0;
};

std::vector<std::string_view> Split(const std::string_view text, bool by_lines) {
    size_t lb = 0, rb = 0;
    std::vector<std::string_view> result;
    while (rb <= text.size()) {
        bool flag = by_lines ? (text[rb] == '\n') : (!std::isalpha(text[rb]));
        if (flag || rb == text.size()) {
            if (rb != lb) {
                result.push_back(text.substr(lb, rb - lb));
            }
            lb = rb = rb + 1;
        } else {
            rb++;
        }
    }

    return result;
}

std::vector<std::string_view> Search(std::string_view text, std::string_view query, size_t results_count) {
    std::unordered_map<std::string_view, WordInfo, CaseInsensitiveStringHash, CaseInsensitiveStringComparator>
        query_word_info;
    for (const auto& query_word : Split(query, false)) {
        query_word_info.emplace(query_word, WordInfo());
    }

    std::vector<std::string_view> lines = Split(text, true);
    std::vector<size_t> line_word_count;
    size_t total_lines = 0;
    for (const auto& line : lines) {
        line_word_count.push_back(0);
        for (const auto& word : Split(line, false)) {
            line_word_count[total_lines]++;
            auto it = query_word_info.find(word);
            if (it != query_word_info.end()) {
                if ((it->second.amount_in_line[total_lines])++ == 0) {
                    (it->second.lines_count)++;
                }
            }
        }
        total_lines++;
    }

    std::vector<std::pair<double, size_t>> line_relevance(total_lines);
    for (size_t i = 0; i < total_lines; ++i) {
        line_relevance[i] = {0, i};
        for (auto& [query_word, info] : query_word_info) {
            (void)query_word;
            double tf = static_cast<double>(info.amount_in_line[i]) / static_cast<double>(line_word_count[i]);
            double idf = std::log(static_cast<double>(total_lines) / static_cast<double>(info.lines_count));
            line_relevance[i].first += tf * idf;
        }
    }

    sort(line_relevance.begin(), line_relevance.end(),
         [](const std::pair<double, size_t>& a, const std::pair<double, size_t>& b) {
             return std::tuple(-a.first, a.second) < std::tuple(-b.first, b.second);
         });

    std::vector<std::string_view> result;
    for (const auto& [tf_idf, ind] : line_relevance) {
        if (result.size() == results_count || tf_idf <= 0) {
            break;
        }
        result.push_back(lines[ind]);
    }

    return result;
}
