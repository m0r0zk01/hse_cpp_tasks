#pragma once

#include <forward_list>
#include <string>

using Value = std::string;
using Iterator = std::forward_list<std::string>::const_iterator;
using ZippedPair = std::pair<const Value&, const Value&>;
using IteratorPair = std::pair<Iterator, Iterator>;

class Zipped;

class ZippedIterator {
public:
    ZippedIterator(IteratorPair it, IteratorPair ends);

    ZippedIterator operator++();
    ZippedIterator operator++(int);

    ZippedPair operator*() const;

    bool operator!=(const ZippedIterator& other) const;

private:
    IteratorPair it_;
    IteratorPair ends_;
};

class Zipped {
public:
    Zipped(Iterator a_begin, Iterator a_end, Iterator b_begin, Iterator b_end);

    ZippedIterator begin() const;
    ZippedIterator end() const;

private:
    ZippedIterator begin_;
    ZippedIterator end_;
};

Zipped Zip(Iterator a_begin, Iterator a_end, Iterator b_begin, Iterator b_end);
