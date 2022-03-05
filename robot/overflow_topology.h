#pragma once

#include "l1_topology.h"

class OverflowTopology : virtual public L1Topology {
public:
    OverflowTopology(const FieldMap& map, const Point& start);
    virtual ~OverflowTopology() = default;

    size_t MeasureDistance(const Point& from, const Point& to) const;
    std::vector<Point> Lookup() const;
    void Move(const Point& point);
    bool CheckMove(int dx, int dy) const;
};
