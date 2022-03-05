#include "hash_table.h"

#include <vector>

int main() {
    std::vector<std::pair<int, int>> v{{1, 2}, {3, 4}};
    std::vector<int> a{1, 2, 3};
    HashTable<int, int> ht(v);
    ht.insert({100, 100});
    return 0;
}