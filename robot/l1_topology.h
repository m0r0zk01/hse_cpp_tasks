#pragma once

#include "field.h"
#include "topology.h"

class L1Topology : virtual public Field, virtual public Topology {
public:
    L1Topology(const Point& start_position);
    virtual ~L1Topology() = default;

    virtual size_t MeasureDistance(const Point& from, const Point& to) const;
    virtual bool CheckMove(int dx, int dy) const;
    virtual Point GetPosition() const;

protected:
    Point position_;
};
