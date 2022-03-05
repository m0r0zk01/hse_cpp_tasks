#include <catch.hpp>

#include "search.h"

TEST_CASE("Search") {
    std::string_view text =
        "Lorem Ipsum is LoREm simply dummy text\n\n\n"
        "of the printing and typesetting industry.\n"
        "Lorem Ipsum has been1234LoReM     the industry's standard\n"
        "dummy text ever since the 1500s, when an unknown\n"
        "printer took a galley of type and scrambled it\n"
        "to make a type specimen book. It has survived\n"
        "not only five centuries, but also the leap into\n"
        "electronic typesetting, remaining essentially\n"
        "unchanged. It was popularised in the 1960s with\n"
        "the release of Letraset sheets containing Lorem\n"
        "psum passages, and more recently with desktop\n"
        "publishing software like Aldus PageMaker\n"
        "including versions of Lorem Ipsum.";

    std::string_view query = "typesetting release";
    std::vector<std::string_view> expected = {"electronic typesetting, remaining essentially"};

    REQUIRE(expected == Search(text, query, 1));
}
