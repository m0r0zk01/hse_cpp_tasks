#include "word2vec.h"
#include <algorithm>
#include <vector>

int64_t ScalarProduct(const std::vector<int>& a, const std::vector<int>& b) {
    int64_t result = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        result += static_cast<int64_t>(a[i]) * static_cast<int64_t>(b[i]);
    }
    return result;
}

std::vector<std::string> FindClosestWords(const std::vector<std::string>& words,
                                          const std::vector<std::vector<int>>& vectors) {
    if (words.size() <= 1) {
        return {};
    }

    std::vector<int64_t> scalar_product(words.size());
    int64_t max_scalar_product = scalar_product[1] = ScalarProduct(vectors[0], vectors[1]);

    for (size_t i = 2; i < vectors.size(); ++i) {
        scalar_product[i] = ScalarProduct(vectors[0], vectors[i]);
        max_scalar_product = std::max(max_scalar_product, scalar_product[i]);
    }

    std::vector<std::string> result;
    for (size_t i = 1; i < words.size(); ++i) {
        if (scalar_product[i] == max_scalar_product) {
            result.push_back(words[i]);
        }
    }

    return result;
}
