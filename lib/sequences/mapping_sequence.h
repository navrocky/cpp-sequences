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

//    using Mapper = std::function<ResultValueType(const SrcValueType&)>;

//    MappingSequence(SrcSequence& srcSequence, const Mapper& mapper)
//        : srcSequence(srcSequence)
//        , mapper(mapper)
//    {
//        std::cout << __FUNCTION__ << std::endl;
//    }

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

template <typename ResultType, typename Mapper>
class MappingSequenceFactory
{
public:
    MappingSequenceFactory(Mapper&& mapper)
        : mapper(mapper)
    {
        std::cout << __FUNCTION__ << std::endl;
    }

    template <typename SrcSequence>
    MappingSequence<SrcSequence, ResultType> create(SrcSequence&& srcSequence) const
    {
        return MappingSequence<SrcSequence, ResultType>(srcSequence, mapper);
        std::cout << __FUNCTION__ << std::endl;
    }

//    template <typename SrcType>
//    MappingSequence<SrcType, ResultType> create(Sequence<SrcType>&& srcSequence) const
//    {
//        return MappingSequence<SrcType, ResultType>(srcSequence, mapper);
//    }

private:
    Mapper mapper;
};

template <typename Mapper>
auto map(const Mapper& mapper)
{
    using ResultType = decltype(resultDeducer(&Mapper::operator()));
    return MappingSequenceFactory<ResultType, Mapper>(mapper);
}

template <typename Sequence, typename Mapper>
auto createMapperSequence(Sequence&& seq, Mapper&& mapper)
{
    return MappingSequence<Sequence, Mapper>(std::move(seq), std::move(mapper));
}

}
