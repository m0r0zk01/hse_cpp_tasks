#include "field.h"

Field::Field(const FieldMap& field) {
    height_ = field.size();
    if (height_) {
        width_ = field.front().size();
    }
    for (size_t i = 0; i < field.size(); ++i) {
        for (size_t j = 0; j < field[i].size(); ++j) {
            if (field[i][j]) {
                walls_.insert({j, i});
            }
        }
    }
}