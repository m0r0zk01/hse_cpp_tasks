#include "password.h"

bool ValidatePassword(const std::string& password) {
    if (password.size() < 8 || password.size() > 14)
        return false;
    bool has_upper = false,
         has_lower = false,
         has_digit = false,
         has_other = false;
    for (char c : password) {
        if (c < 33 || c > 126)
            return false;
        if (isupper(c))
            has_upper = true;
        else if (islower(c))
            has_lower = true;
        else if (isdigit(c))
            has_digit = true;
        else
            has_other = true;
    }
    return static_cast<int>(has_upper) +
           static_cast<int>(has_lower) +
           static_cast<int>(has_digit) +
           static_cast<int>(has_other) >= 3;
}
