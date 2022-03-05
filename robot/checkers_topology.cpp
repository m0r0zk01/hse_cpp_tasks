#include "checkers_topology.h"

CheckersTopology::CheckersTopology(const FieldMap& map, const Point& start) : Field(map), L1Topology(start) {
}

std::vector<Point> CheckersTopology::Lookup() const {
    std::vector<Point> result;
    for (auto [dx, dy] : std::vector<std::pair<int, int>>{{1, 1}, {1, -1}, {-1, -1}, {-1, 1}}) {
        if (CheckMove(dx, dy)) {
            result.push_back({position_.x + dx, position_.y + dy});
        } else if (CheckMove(2 * dx, 2 * dy)) {
            result.push_back({position_.x + 2 * dx, position_.y + 2 * dy});
        }
    }
    return result;
}

void CheckersTopology::Move(const Point& point) {
    position_ = point;
}
