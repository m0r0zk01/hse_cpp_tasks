#include "knight_topology.h"

KnightTopology::KnightTopology(const FieldMap& map, const Point& start) : Field(map), L1Topology(start) {
}

std::vector<Point> KnightTopology::Lookup() const {
    std::vector<Point> result;
    for (auto [dx, dy] :
         std::vector<std::pair<int, int>>{{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}}) {
        if (CheckMove(dx, dy)) {
            result.push_back({position_.x + dx, position_.y + dy});
        }
    }
    return result;
}

void KnightTopology::Move(const Point& point) {
    position_ = point;
}
