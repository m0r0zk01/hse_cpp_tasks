#include "palindrome.h"

bool IsPalindrome(const std::string& str) {
    if (str.empty()) {
        return true;
    }
    size_t l = 0, 
           r = str.size() - 1;
    while (l < r) {
        while (l < r && str[l] == ' ') {
            l++;
        }
        while (r > l && str[r] == ' ') {
            r--;
        }
        if (l >= r) {
            break;
        }
        if (str[l] != str[r]) {
            return false;
        }
        l++;
        r--;
    }
    return true;
}
