#include "poly.h"

#include <algorithm>
#include <cmath>
#include <ostream>

Poly::Poly(std::vector<int64_t> coefficients) {
    for (size_t i = 0; i < coefficients.size(); ++i) {
        if (coefficients[i]) {
            poly_.emplace(i, coefficients[i]);
        }
    }
}

Poly::Poly(std::vector<Coefficient> coefficients) {
    for (const auto& coefficient : coefficients) {
        if (coefficient.second) {
            poly_.insert(coefficient);
        }
    }
}

Poly::Poly(const Poly& poly) : poly_(poly.poly_) {
}

int64_t Poly::operator()(int64_t x) const {
    int64_t result = 0;
    for (const auto& [power, coefficient] : poly_) {
        result += coefficient * static_cast<int64_t>(std::pow(x, power));
    }
    return result;
}

Poly Poly::operator+(const Poly& other) const {
    Poly new_poly(*this);
    for (const auto& [power, coefficient] : other.poly_) {
        auto& ref = new_poly.poly_[power];
        ref += coefficient;
        if (ref == 0) {
            new_poly.poly_.erase(power);
        }
    }
    return new_poly;
}

Poly Poly::operator-(const Poly& other) const {
    Poly new_poly(*this);
    for (const auto& [power, coefficient] : other.poly_) {
        auto& ref = new_poly.poly_[power];
        ref -= coefficient;
        if (ref == 0) {
            new_poly.poly_.erase(power);
        }
    }
    return new_poly;
}

Poly Poly::operator*(const Poly& other) const {
    Poly new_poly;
    for (const auto& [power1, coefficient1] : this->poly_) {
        for (const auto& [power2, coefficient2] : other.poly_) {
            new_poly.poly_[power1 + power2] += coefficient1 * coefficient2;
        }
    }
    for (const auto& [power, coefficient] : new_poly.poly_) {
        if (!coefficient) {
            new_poly.poly_.erase(power);
        }
    }
    return new_poly;
}

Poly Poly::operator-() const {
    Poly new_poly(*this);
    for (auto& it : new_poly.poly_) {
        it.second *= -1;
    }
    return new_poly;
}

Poly& Poly::operator+=(const Poly& other) {
    for (const auto& [power, coefficient] : other.poly_) {
        auto& ref = this->poly_[power];
        ref += coefficient;
        if (!ref) {
            this->poly_.erase(power);
        }
    }
    return *this;
}

Poly& Poly::operator-=(const Poly& other) {
    for (const auto& [power, coefficient] : other.poly_) {
        auto& ref = this->poly_[power];
        ref -= coefficient;
        if (!ref) {
            this->poly_.erase(power);
        }
    }
    return *this;
}

bool Poly::operator==(const Poly& other) const {
    return this->poly_ == other.poly_;
}

bool Poly::operator!=(const Poly& other) const {
    return this->poly_ != other.poly_;
}

std::ostream& operator<<(std::ostream& os, const Poly& poly) {
    os << "y = ";
    if (poly.poly_.empty()) {
        os << 0;
        return os;
    }
    for (auto it = poly.poly_.rbegin(); it != --poly.poly_.rend(); ++it) {
        os << it->second << "x^" << it->first << " + ";
    }
    os << poly.poly_.begin()->second;
    return os;
}
