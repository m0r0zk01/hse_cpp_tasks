#pragma once

#include "l1_topology.h"

class KnightTopology : virtual public L1Topology {
public:
    KnightTopology(const FieldMap& map, const Point& start);
    virtual ~KnightTopology() = default;

    std::vector<Point> Lookup() const;
    void Move(const Point& point);
};
