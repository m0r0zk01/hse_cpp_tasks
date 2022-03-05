#pragma once

#include "l1_topology.h"

class PlanarTopology : virtual public L1Topology {
public:
    PlanarTopology(const FieldMap& map, const Point& start);
    virtual ~PlanarTopology() = default;

    std::vector<Point> Lookup() const;
    void Move(const Point& point);
};
