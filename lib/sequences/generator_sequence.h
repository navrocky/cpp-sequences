#pragma once

#include "../common/sequence.h"

namespace Sequences
{

namespace Internal
{
template <typename T, typename Arg>
Arg generatorArgDeducer(bool (T::*)(Arg&) const)
{
}
}

template <typename Generator,
          typename ResultValueType = decltype(Internal::generatorArgDeducer(&Generator::operator()))>
class GeneratorSequence : public Sequence<ResultValueType>
{
public:
    using ValueType = ResultValueType;
    GeneratorSequence(const Generator& generator)
        : generator(generator)
    {
    }

    bool getNextValue(ResultValueType& v) { return generator(v); }

private:
    Generator generator;
};

template <typename Generator>
auto sequenceGenerator(const Generator& generator)
{
    return GeneratorSequence<Generator>(generator);
}
}
