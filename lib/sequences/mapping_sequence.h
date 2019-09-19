#pragma once

#include "sequence.h"
#include <functional>

namespace Sequences
{

template <typename Ret, typename T, typename Arg>
Ret mapperResultDeducer(Ret (T::*)(Arg) const)
{
}

template <typename SrcSequence, typename Mapper, typename ResultValueType = decltype(mapperResultDeducer(&Mapper::operator()))>
class MappingSequence : public Sequence<ResultValueType>
{
public:
    using SrcValueType = typename SrcSequence::ValueType;

    MappingSequence(SrcSequence&& srcSequence, Mapper&& mapper)
        : srcSequence(std::move(srcSequence))
        , mapper(std::move(mapper))
    {
        std::cout << __FUNCTION__ << std::endl;
    }

    MappingSequence(MappingSequence<SrcSequence, Mapper>&& src)
        : srcSequence(std::move(src.srcSequence))
        , mapper(std::move(src.mapper))
    {
        std::cout << __FUNCTION__ << std::endl;
    }

    ~MappingSequence()
    {
        std::cout << __FUNCTION__ << std::endl;
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
class MappingSequenceFactory
{
public:
    MappingSequenceFactory(Mapper&& mapper)
        : mapper(std::move(mapper))
    {
        std::cout << __FUNCTION__ << std::endl;
    }

    MappingSequenceFactory(const MappingSequenceFactory<Mapper>& src) = delete;

    MappingSequenceFactory(MappingSequenceFactory<Mapper>&& src)
        : mapper(std::move(src.mapper))
    {
    }

    template <typename SrcSequence>
    MappingSequence<SrcSequence, Mapper> create(SrcSequence&& srcSequence) const
    {
        return MappingSequence<SrcSequence, Mapper>(std::move(srcSequence), Mapper(mapper));
        std::cout << __FUNCTION__ << std::endl;
    }

private:
    Mapper mapper;
};

template <typename Mapper>
auto map(Mapper&& mapper)
{
    return MappingSequenceFactory<Mapper>(std::move(mapper));
}

template <typename Sequence, typename Mapper>
auto createMappingSequence(Sequence&& seq, Mapper&& mapper)
{
    return MappingSequence<Sequence, Mapper>(std::move(seq), std::move(mapper));
}

}
