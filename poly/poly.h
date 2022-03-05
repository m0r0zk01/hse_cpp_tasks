#pragma once

#include <map>
#include <ostream>
#include <vector>

class Poly {
public:
    using Coefficient = std::pair<int64_t, int64_t>;

    Poly() = default;
    Poly(std::vector<int64_t> coefficients);
    Poly(std::vector<Coefficient> coefficients);
    Poly(const Poly& poly);

    int64_t operator()(int64_t x) const;

    Poly operator+(const Poly& other) const;
    Poly operator-(const Poly& other) const;
    Poly operator*(const Poly& other) const;
    Poly operator-() const;
    Poly& operator+=(const Poly& other);
    Poly& operator-=(const Poly& other);

    bool operator==(const Poly& other) const;
    bool operator!=(const Poly& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Poly& poly);

private:
    std::map<int64_t, int64_t> poly_;
};
