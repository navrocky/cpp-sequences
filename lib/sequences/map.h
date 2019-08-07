#pragma once

#include "sequence.h"
#include <functional>

namespace Sequences
{

template <typename SrcValueType, typename ResultValueType>
class MappingSequence : public Sequence<ResultValueType>
{
public:
    using Mapper = std::function<ResultValueType(const SrcValueType&)>;

    MappingSequence(Sequence<SrcValueType>& srcSequence, const Mapper& mapper) {}

private:
    Sequence<SrcValueType>& srcSequence;
    Mapper mapper;
};

template <typename ResultType, typename Mapper>
class MappingSequenceFactory
{
public:
    MappingSequenceFactory(const Mapper& mapper)
        : mapper(mapper)
    {
    }

    template <typename SrcType>
    MappingSequence<SrcType, ResultType> create(const Sequence<SrcType>& srcSequence)
    {
        throw 0;
    }

private:
    Mapper mapper;
};

template <typename Mapper>
auto map(const Mapper& mapper)
{
    using ResultType = decltype(mapper());
    return MappingSequenceFactory<ResultType, Mapper>(mapper);
}
}
