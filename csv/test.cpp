#include "csv_parser.h"

#include <fstream>
#include <iostream>
#include <string>

int main() {
    std::string filename = "test.csv";
    std::ifstream is(filename, std::ios::binary);
    CsvParser<std::string, double, double, double> parser(is, ';', 2);
    for (const auto& line : parser) {
        std::cout << line << '\n';
    }
    return 0;
}