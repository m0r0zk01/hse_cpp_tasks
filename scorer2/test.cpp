#include <catch.hpp>
#include <iostream>
#include "scorer.h"

void PrintScorer(const Scorer& scorer) {
    auto score_table = scorer.GetScoreTable();
    for (const auto& i : score_table) {
        std::cout << i.first << ':';
        for (const auto& j : i.second) {
            std::cout << j << ' ';
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

TEST_CASE("Scorer") {
    Scorer scorer;

    scorer.OnCheckSuccess("Popov", "old");
    PrintScorer(scorer);
    scorer.Reset();
    PrintScorer(scorer);
    scorer.OnCheckSuccess("Ivanov", "utf8");
    PrintScorer(scorer);
    scorer.OnCheckSuccess("Ivanov", "scorer");
    PrintScorer(scorer);
    scorer.OnCheckSuccess("Petrov", "utf8");
    PrintScorer(scorer);
    scorer.OnCheckFailed("Petrov", "scorer");
    PrintScorer(scorer);
    scorer.OnCheckFailed("Sidorov", "scorer");
    PrintScorer(scorer);

    const ScoreTable expected_result = {
        {"Ivanov", {"utf8", "scorer"}},
        {"Petrov", {"utf8"}},
    };

    REQUIRE(expected_result == scorer.GetScoreTable());
}
