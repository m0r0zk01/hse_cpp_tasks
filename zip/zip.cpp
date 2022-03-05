#include "zip.h"

#include <iostream>

ZippedIterator::ZippedIterator(IteratorPair it, IteratorPair ends) :
      it_(it), ends_(ends) {}

ZippedIterator Zipped::begin() const {
    return begin_;
}

ZippedIterator Zipped::end() const {
    return end_;
}

ZippedIterator ZippedIterator::operator++() {  // ++it
    if (++it_.first == ends_.first || ++it_.second == ends_.second) {
        it_ = ends_;
    }
    return *this;
}

ZippedIterator ZippedIterator::operator++(int) {  // it++
    ZippedIterator old = *this;
    if (++it_.first == ends_.first || ++it_.second == ends_.second) {
        it_ = ends_;
    }
    return old;
}

ZippedPair ZippedIterator::operator*() const {
    return {*it_.first, *it_.second};
}

bool ZippedIterator::operator!=(const ZippedIterator& other) const {
    return this->it_ != other.it_;
}

Zipped::Zipped(Iterator a_begin, Iterator a_end, Iterator b_begin, Iterator b_end) :
      begin_({a_begin, b_begin}, {a_end, b_end}),
      end_({a_end, b_end}, {a_end, b_end}) {}

Zipped Zip(Iterator a_begin, Iterator a_end, Iterator b_begin, Iterator b_end) {
    return Zipped(a_begin, a_end, b_begin, b_end);
}
