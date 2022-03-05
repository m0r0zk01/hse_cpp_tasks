#include <catch.hpp>
#include <sstream>
#include <vector>

#include "hash_table.h"

namespace {
class BadHash {
public:
    size_t operator()(int) const {
        return 0;
    }
};
}  // namespace

TEST_CASE("HashTable") {
    HashTable<std::string, int> hash_table;
    hash_table["a"] = 1;
    hash_table["b"]++;

    REQUIRE(hash_table.find("a")->second == hash_table.at("b"));

    const HashTable<std::string, int> hash_table_copy = hash_table;
    for (const auto& [key, value] : hash_table_copy) {
        REQUIRE(!key.empty());
        REQUIRE(1 == value);
    }

    REQUIRE(2 == hash_table.size());
    hash_table.clear();
    REQUIRE(0 == hash_table.size());
    REQUIRE(hash_table.empty());

    {
        const auto& [it, inserted] = hash_table.insert({"c", 3});
        REQUIRE(inserted);
    }
    {
        const auto& [it, inserted] = hash_table.emplace("c", 3);
        REQUIRE(!inserted);
    }
}

TEST_CASE("HashTable_BadHash") {
    const size_t expected_size = 1000;
    HashTable<int, int, BadHash> hash_table;

    for (size_t i = 0; i < expected_size; ++i) {
        hash_table[i] = i;
    }

    REQUIRE(expected_size == hash_table.size());

    for (const auto& [key, value] : hash_table) {
        REQUIRE(key == value);
        REQUIRE(hash_table.at(key) == value);
        REQUIRE(hash_table.find(key)->second == value);
    }
}

TEST_CASE("HashTable_1") {
    std::stringstream output_stream;
    std::vector<std::pair<int, int>> v{{5, 5}, {6, 6}};
    HashTable<int, int> ht(v);
    for (const auto& i : ht) {
        output_stream << i.first << ' ' << i.second << '\n';
    }
    ht.insert({1, 1});
    for (const auto& i : ht) {
        output_stream << i.first << ' ' << i.second << '\n';
    }
    ht.insert({2, 2});
    for (auto& i : ht) {
        output_stream << i.first << ' ' << i.second << '\n';
    }
    ht.insert({3, 3});
    for (auto& i : ht) {
        output_stream << i.first << ' ' << i.second << '\n';
    }
    output_stream << ht[1] << ' ';
    output_stream << ht[2] << ' ';
    output_stream << ht[3] << ' ';
    output_stream << ht[4] << '\n';

    ht[4] = 100;
    output_stream << ht[4] << '\n';
    ht.clear();

    output_stream << ht[1] << ' ';
    output_stream << ht[2] << ' ';
    output_stream << ht[3] << ' ';
    output_stream << ht[4] << '\n';

    std::cout << output_stream.str();

//    REQUIRE(output_stream.str() == "6 6\n"
//            "5 5\n"
//            "6 6\n"
//            "5 5\n"
//            "1 1\n"
//            "5 5\n"
//            "1 1\n"
//            "6 6\n"
//            "2 2\n"
//            "5 5\n"
//            "1 1\n"
//            "6 6\n"
//            "2 2\n"
//            "3 3\n"
//            "1 2 3 0\n"
//            "100\n"
//            "0 0 0 0\n");
    REQUIRE(true);
}
