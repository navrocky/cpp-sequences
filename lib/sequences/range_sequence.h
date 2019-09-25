#pragma once

#include "../common/sequence.h"

namespace Sequences
{

template <typename T>
class RangeSequence : public Sequence<T>
{
public:
    using ValueType = T;

    RangeSequence(T from, T to)
        : current(from)
        , to(to)
    {
    }

    bool getNextValue(T& v)
    {
        if (current >= to)
            return false;
        v = current;
        ++current;
        return true;
    }

private:
    T current;
    T to;
};

template <typename ValueType>
auto sequenceFromRangeExcl(ValueType from, ValueType to)
{
    return RangeSequence<ValueType>(from, to);
}

template <typename ValueType>
auto sequenceFromRangeIncl(ValueType from, ValueType to)
{
    return RangeSequence<ValueType>(from, ++to);
}
}
