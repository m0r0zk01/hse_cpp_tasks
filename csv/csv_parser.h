#pragma once

#include <exception>
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <tuple>

class CsvParserError : public std::exception {};

template <typename T>
T ParseValue(const std::string_view str);

template <>
int ParseValue<int>(const std::string_view str) {
    if (!std::all_of(str.begin(), str.end(), [](char c) { return std::isdigit(c); })) {
        throw CsvParserError();
    }
    return str.empty() ? 0 : atoi(str.data());
}

template <>
std::string ParseValue<std::string>(const std::string_view str) {
    return str.data();
}

template <typename... T>
class CsvParser {
public:
    class Iterator {
    public:
        friend class CsvParser;
        explicit Iterator(CsvParser& parent) : parent_(parent) {
        }

        Iterator operator++() {
            return *this;
        }

        bool operator!=(const Iterator& other) {
            return !parent_.is_.eof();
        }

        template <size_t n>
        typename std::enable_if<n >= sizeof...(T)>::type FillTuple(std::tuple<T...>& t) {
        }

        template <size_t n>
            typename std::enable_if < n<sizeof...(T)>::type FillTuple(std::tuple<T...>& t) {
            using CurType = typename std::tuple_element<n, std::tuple<T...>>::type;
            bool is_escaped = false;
            bool double_quote = false;
            std::string cur_value;
            char c;

            while (parent_.is_.get(c)) {
                if ((c == '\n' || c == parent_.sep_) && !is_escaped) {
                    break;
                } else if (c == '"') {
                    if (cur_value.empty()) {
                        is_escaped = true;
                    } else if (double_quote) {
                        double_quote = false;
                    } else if (parent_.is_.peek() == '"') {
                        cur_value.push_back(c);
                        double_quote = true;
                    } else if (is_escaped && (parent_.is_.peek() == parent_.sep_ || parent_.is_.peek() == '\n')) {
                        is_escaped = false;
                    }
                } else {
                    cur_value.push_back(c);
                }
            }
            std::get<n>(t) = ParseValue<CurType>(cur_value);
            FillTuple<n + 1>(t);
        }

        std::tuple<T...> operator*() {
            std::tuple<T...> result;
            FillTuple<0>(result);
            return result;
        }

    private:
        CsvParser& parent_;
    };

    Iterator begin() {
        return Iterator(*this);
    }

    Iterator end() {
        return Iterator(*this);
    }

    CsvParser(std::istream& is, char sep = ',', size_t skip = 0) : is_(is), sep_(sep) {
        for (size_t i = 0; i < skip; ++i) {
            if (!is.ignore(std::numeric_limits<std::streamsize>::max(), '\n')) {
                throw CsvParserError();
            }
        }
    }

private:
    std::istream& is_;
    char sep_ = ',';
};

template <size_t n, typename... T>
typename std::enable_if<n >= sizeof...(T)>::type PrintTuple(std::ostream& os, const std::tuple<T...>& t) {
}

template <size_t n, typename... T>
    typename std::enable_if < n<sizeof...(T)>::type PrintTuple(std::ostream& os, const std::tuple<T...>& t) {
    if (n == sizeof...(T)) {
        return;
    } else if (n) {
        os << ", ";
    }
    os << std::get<n>(t);
    PrintTuple<n + 1>(os, t);
}

template <typename... T>
std::ostream& operator<<(std::ostream& os, const std::tuple<T...>& t) {
    PrintTuple<0>(os, t);
    return os;
}
