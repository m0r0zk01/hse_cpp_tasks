#pragma once

#include <cstddef>
#include <unordered_set>

struct Point {
    size_t x = 0;
    size_t y = 0;
};

struct PointHash {
    size_t operator()(const Point& obj) const {
        return obj.x + 0x9e3779b9 + (obj.y << 6) + (obj.y >> 2);
    }
};

struct PointEqual {
    bool operator()(const Point& a, const Point& b) const {
        return a.x == b.x && a.y == b.y;
    }
};

using PointSet = std::unordered_set<Point, PointHash, PointEqual>;