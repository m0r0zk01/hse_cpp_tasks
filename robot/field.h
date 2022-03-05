#pragma once

#include "point.h"

#include <vector>
#include <unordered_set>

using FieldMap = std::vector<std::vector<bool>>;

class Field {
public:
    Field(const FieldMap& field);
    virtual ~Field() = default;

protected:
    size_t height_ = 0;
    size_t width_ = 0;
    PointSet walls_;
};
