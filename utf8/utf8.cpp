#include "utf8.h"
#include <iostream>
#include <cstddef>
#include <vector>
#include <string>

uint32_t getNBit(uint32_t num, uint32_t offset, uint32_t n) {
    uint32_t mask = ((1 << n) - 1) << offset;
    return ((num & mask) >> offset);
}

void EncodeUtf8(const std::vector<uint32_t>& codepoints, std::string& str) {  
    for (uint32_t c : codepoints) {
        if (c <= (1 << 7)) {
            str += c;
        } else if (c <= (1 << 11)) {
            str += (0b110 << 5) + getNBit(c, 6, 5);
            str += (0b10 << 6) + getNBit(c, 0, 6);
        } else if (c <= (1 << 16)) {
            str += (0b1110 << 4) + getNBit(c, 12, 4);
            str += (0b10 << 6) + getNBit(c, 6, 6);
            str += (0b10 << 6) + getNBit(c, 0, 6);
        } else {
            str += (0b11110 << 3) + getNBit(c, 18, 3);
            str += (0b10 << 6) + getNBit(c, 12, 6);
            str += (0b10 << 6) + getNBit(c, 6, 6);
            str += (0b10 << 6) + getNBit(c, 0, 6);
        }
    }
}

void DecodeUtf8(const std::string& str, std::vector<uint32_t>& codepoints) {
    for (size_t i = 0; i < str.size(); i++) {
        uint32_t tmp = 0;
        if (getNBit(str[i], 7, 1) == 0) {
            tmp += str[i];
        } else if (getNBit(str[i], 5, 3) == 6) {
            tmp += getNBit(str[i + 0], 0, 5) << 6;
            tmp += getNBit(str[i + 1], 0, 6);
            i += 1;
        } else if (getNBit(str[i], 4, 4) == 14) {
            tmp += getNBit(str[i + 0], 0, 4) << 12;
            tmp += getNBit(str[i + 1], 0, 6) << 6;
            tmp += getNBit(str[i + 2], 0, 6);
            i += 2;
        } else {
            tmp += getNBit(str[i + 0], 0, 3) << 18;
            tmp += getNBit(str[i + 1], 0, 6) << 12;
            tmp += getNBit(str[i + 2], 0, 6) << 6;
            tmp += getNBit(str[i + 3], 0, 6);
            i += 3;
        }
        codepoints.push_back(tmp);
    }
}

size_t CountUtf8Symbols(const std::string& str) {
    std::vector<uint32_t> tmp_codepoints;
    DecodeUtf8(str, tmp_codepoints);
    return tmp_codepoints.size();
}
