#include <catch.hpp>

#include "cow_string.h"

#include <cctype>
#include <iostream>

TEST_CASE("CowString") {
    const CowString str1("hello");
    CowString str2 = str1;

    REQUIRE(str1.GetData() == str2.GetData());

    str2[0] = 'h';
    REQUIRE(str1.GetData() == str2.GetData());

    str2 += " world";
    REQUIRE(str1.GetData() != str2.GetData());

    for (const auto ch : str1) {
        REQUIRE(std::isalpha(ch));
    }

    const auto* const str2_data = str2.GetData();
    str2[5] = '_';
    REQUIRE("hello_world" == str2);
    REQUIRE(str2 == "hello_world");
    REQUIRE(str2_data == str2.GetData());

    str2 = str1;
    REQUIRE("hello" == str2);
    REQUIRE(str1.GetData() == str2.GetData());

    const CowString& const_str2 = str2;
    REQUIRE('e' == const_str2.At(1));
    {
        auto it1 = str1.begin();
        auto it2 = str2.begin();
        auto const_it2 = const_str2.begin();
        *it2 = 'H';
        REQUIRE("Hello" == str2);
        REQUIRE("Hello" != str1);
        REQUIRE('h' == *it1);
        REQUIRE('H' == *it2);
        REQUIRE('H' == *const_it2);
    }
    std::string_view a = str2;
    REQUIRE(a == "Hello");
}

TEST_CASE("CowString1") {
    CowString s1("hello");
    CowString s2("aboba");
    CowString s3(s1 + s2);
    auto it = s3.begin();
    ++it;
    ++it;
    REQUIRE(*it == 'l');
    s3 += s1;
    REQUIRE(*it == 'l');
    REQUIRE(s3 == "helloabobahello");

    {
        CowString s4(s3);
        CowString s5(s4);
        CowString s6(s5);
//        s3[0] = 'H';
        s5[0] = 'A';
        REQUIRE(s4.GetData() != s5.GetData());
    }
    REQUIRE(s3 == "helloabobahello");
}

TEST_CASE("CowString2") {
    CowString s1("hello");
    CowString s2("aboba");
    REQUIRE(s1 + s2 == "helloaboba");
    s2 += "!";
    s1 = s2;
    REQUIRE(s1 + s2 == "aboba!aboba!");
}