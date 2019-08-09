#pragma once

#include "sequence.h"

namespace Sequences
{

template <typename Iterable>
class IterableSequence : public Sequence<typename Iterable::value_type>
{
public:
    using ValueType = typename Iterable::value_type;

    IterableSequence(const Iterable& iterable)
        : iterable(iterable)
    {
    }

    bool getNextValue(ValueType& v) override { return true; }

private:
    const Iterable& iterable;
};

template <typename Iterable>
auto sequenceFromIterable(const Iterable& iterable)
{
    return IterableSequence<Iterable>(iterable);
}
}
