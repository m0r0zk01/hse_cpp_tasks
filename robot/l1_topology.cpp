#include "l1_topology.h"

L1Topology::L1Topology(const Point& start_position) : position_(start_position) {
}

size_t L1Topology::MeasureDistance(const Point& from, const Point& to) const {
    return std::max(from.x, to.x) - std::min(from.x, to.x) + std::max(from.y, to.y) - std::min(from.y, to.y);
}

bool L1Topology::CheckMove(int dx, int dy) const {
    bool check_x = (dx > 0) ? (position_.x + dx < +width_) : (position_.x >= abs(dx));
    bool check_y = (dy > 0) ? (position_.y + dy < height_) : (position_.y >= abs(dy));
    return check_x && check_y && walls_.find({position_.x + dx, position_.y + dy}) == walls_.end();
}

Point L1Topology::GetPosition() const {
    return position_;
}
