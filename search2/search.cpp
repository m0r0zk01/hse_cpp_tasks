#include "search.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <tuple>

std::string GetLowercase(const std::string_view s) {
    std::string lowercase_string;
    for (const auto c : s) {
        lowercase_string.push_back(std::tolower(c));
    }
    return lowercase_string;
}

size_t CaseInsensitiveStringHash::operator()(const std::string_view obj) const {
    std::hash<std::string> hash;
    return hash(GetLowercase(obj));
}

bool CaseInsensitiveStringComparator::operator()(const std::string_view a, const std::string_view b) const {
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

std::vector<std::string_view> SearchEngine::Split(const std::string_view text, bool by_lines) const {
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

void SearchEngine::BuildIndex(std::string_view text) {
    total_lines = 0;
    word_info_.clear();
    line_word_count_.clear();
    lines = Split(text, true);

    for (const auto line : lines) {
        line_word_count_.push_back(0);
        for (const auto word : Split(line, false)) {
            line_word_count_[total_lines]++;
            WordInfo& word_info_ref = word_info_[word];
            if (word_info_ref.amount_in_line[total_lines]++ == 0) {
                word_info_ref.lines_count++;
            }
        }
        total_lines++;
    }
}

std::vector<std::string_view> SearchEngine::Search(std::string_view query, size_t results_count) const {
    std::unordered_set<std::string_view, CaseInsensitiveStringHash, CaseInsensitiveStringComparator> used;
    std::vector<std::pair<double, size_t>> line_relevance(total_lines);
    for (auto query_word : Split(query, false)) {
        bool inserted = false;
        std::tie(std::ignore, inserted) = used.emplace(query_word);
        if (inserted) {
            auto it = word_info_.find(query_word);
            for (size_t i = 0; i < total_lines; ++i) {
                line_relevance[i].second = i;
                auto info = it->second.amount_in_line.find(i);
                size_t amount_in_line = (info == it->second.amount_in_line.end()) ? 0 : info->second;
                double tf = static_cast<double>(amount_in_line) / static_cast<double>(line_word_count_[i]);
                double idf = std::log(static_cast<double>(total_lines) / static_cast<double>(it->second.lines_count));
                line_relevance[i].first += tf * idf;
            }
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