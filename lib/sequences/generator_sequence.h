#pragma once

#include "../common/sequence.h"

namespace Sequences
{

namespace Internal
{
template <typename Ret, typename T>
Ret generatorResultDeducer(Ret (T::*)() const)
{
}
}

template <typename T, >
class GeneratorSequence : public Sequence<T>
{
};

template <typename Iterable>
auto generateSequence(const Iterable& iterable)
{
    return RefIterableSequence<Iterable>(iterable);
}
}
