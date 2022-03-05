#include "overflow_topology.h"

OverflowTopology::OverflowTopology(const FieldMap& map, const Point& start) : Field(map), L1Topology(start) {
}

size_t OverflowTopology::MeasureDistance(const Point& from, const Point& to) const {
    size_t x_dist_1 = std::max(from.x, to.x) - std::min(from.x, to.x);
    size_t y_dist_1 = std::max(from.y, to.y) - std::min(from.y, to.y);
    size_t x_dist_2 = (width_ - std::max(from.x, to.x) + std::min(from.x, to.x));
    size_t y_dist_2 = (width_ - std::max(from.y, to.y) + std::min(from.y, to.y));
    return std::min(x_dist_1, x_dist_2) + std::min(y_dist_1, y_dist_2);
}

bool OverflowTopology::CheckMove(int dx, int dy) const {
    size_t dest_x = (position_.x + width_ + dx) % width_;
    size_t dest_y = (position_.y + height_ + dy) % height_;
    return walls_.find({dest_x, dest_y}) == walls_.end();
}

std::vector<Point> OverflowTopology::Lookup() const {
    std::vector<Point> result;
    for (auto [dx, dy] : std::vector<std::pair<int, int>>{{1, 0}, {-1, 0}, {0, 1}, {0, -1}}) {
        size_t dest_x = (position_.x + width_ + dx) % width_;
        size_t dest_y = (position_.y + height_ + dy) % height_;
        if (CheckMove(dx, dy)) {
            result.push_back({dest_x, dest_y});
        }
    }
    return result;
}

void OverflowTopology::Move(const Point& point) {
    position_ = point;
}
