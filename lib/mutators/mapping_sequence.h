#pragma once

#include <functional>

#include "../common/sequence.h"

namespace Sequences
{

template <typename Ret, typename T, typename Arg>
Ret mapperResultDeducer(Ret (T::*)(Arg) const)
{
}

template <typename SrcSequence, typename Mapper,
          typename ResultValueType = decltype(mapperResultDeducer(&Mapper::operator()))>
class MappingSequence : public Sequence<ResultValueType>
{
public:
    using SrcValueType = typename SrcSequence::ValueType;

    MappingSequence(SrcSequence&& srcSequence, Mapper&& mapper)
        : srcSequence(std::move(srcSequence))
        , mapper(std::move(mapper))
    {
    }

    MappingSequence(MappingSequence<SrcSequence, Mapper>&& src)
        : srcSequence(std::move(src.srcSequence))
        , mapper(std::move(src.mapper))
    {
    }

    bool getNextValue(ResultValueType& v)
    {
        SrcValueType srcValue;
        bool hasNext = srcSequence.getNextValue(srcValue);
        if (!hasNext)
            return false;
        v = mapper(srcValue);
        return true;
    }

private:
    SrcSequence srcSequence;
    Mapper mapper;
};

template <typename Mapper>
class MappingSequenceMutator
{
public:
    MappingSequenceMutator(Mapper&& mapper)
        : mapper(std::move(mapper))
    {
    }

    MappingSequenceMutator(const MappingSequenceMutator<Mapper>& src) = delete;

    MappingSequenceMutator(MappingSequenceMutator<Mapper>&& src)
        : mapper(std::move(src.mapper))
    {
    }

    template <typename SrcSequence>
    MappingSequence<SrcSequence, Mapper> create(SrcSequence&& srcSequence) const
    {
        return MappingSequence<SrcSequence, Mapper>(std::move(srcSequence), Mapper(mapper));
    }

private:
    Mapper mapper;
};

template <typename Mapper>
auto map(Mapper&& mapper)
{
    return MappingSequenceMutator<Mapper>(std::move(mapper));
}
}
