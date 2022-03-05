#pragma once

#include "l1_topology.h"

class CheckersTopology : virtual public L1Topology {
public:
    CheckersTopology(const FieldMap& map, const Point& start);
    virtual ~CheckersTopology() = default;

    std::vector<Point> Lookup() const;
    void Move(const Point& point);
};
