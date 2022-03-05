#include "planar_topology.h"

PlanarTopology::PlanarTopology(const FieldMap& map, const Point& start) : Field(map), L1Topology(start) {
}

std::vector<Point> PlanarTopology::Lookup() const {
    std::vector<Point> result;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (abs(dx) + abs(dy) == 1 && CheckMove(dx, dy)) {
                result.push_back({position_.x + dx, position_.y + dy});
            }
        }
    }
    return result;
}

void PlanarTopology::Move(const Point& point) {
    position_ = point;
}
