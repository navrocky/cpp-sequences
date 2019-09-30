#pragma once
#include "../common/sequence.h"

namespace Sequences
{
namespace Internal
{

template <typename SrcSequence, typename Filter>
class RefFilterSequence : public Sequence<typename SrcSequence::ValueType>
{
public:
    using ValueType = typename SrcSequence::ValueType;
    using ValueTypeConstRef = const ValueType*;

    RefFilterSequence(SrcSequence&& srcSequence, Filter&& mapper)
        : srcSequence(std::move(srcSequence))
        , filter(std::move(mapper))
    {
    }

    RefFilterSequence(RefFilterSequence<SrcSequence, Filter>&& src)
        : srcSequence(std::move(src.srcSequence))
        , filter(std::move(src.filter))
    {
    }

    bool getNextValueRef(ValueTypeConstRef& v)
    {
        ValueTypeConstRef val;
        while (srcSequence.getNextValueRef(val))
        {
            if (filter(*val))
            {
                v = val;
                return true;
            }
        }
        return false;
    }

private:
    SrcSequence srcSequence;
    Filter filter;
};

template <typename SrcSequence, typename Filter>
class CopyFilterSequence : public Sequence<typename SrcSequence::ValueType>
{
public:
    using ValueType = typename SrcSequence::ValueType;
    using ValueTypeConstRef = const ValueType*;

    CopyFilterSequence(SrcSequence&& srcSequence, Filter&& mapper)
        : srcSequence(std::move(srcSequence))
        , filter(std::move(mapper))
    {
    }

    CopyFilterSequence(CopyFilterSequence<SrcSequence, Filter>&& src)
        : srcSequence(std::move(src.srcSequence))
        , filter(std::move(src.filter))
    {
    }

    bool getNextValue(ValueType& v)
    {
        ValueType val;
        while (srcSequence.getNextValue(val))
        {
            if (filter(val))
            {
                v = val;
                return true;
            }
        }
        return false;
    }

private:
    SrcSequence srcSequence;
    Filter filter;
};

template <typename Filter>
class FilterSequenceMutator
{
public:
    FilterSequenceMutator(Filter&& filter)
        : filter(std::move(filter))
    {
    }

    FilterSequenceMutator(const FilterSequenceMutator<Filter>& src) = delete;

    FilterSequenceMutator(FilterSequenceMutator<Filter>&& src)
        : filter(std::move(src.filter))
    {
    }

    template <typename SrcSequence,
              std::enable_if_t<Internal::HasGetNextValueRef<SrcSequence>::value, int> = 0>
    auto create(SrcSequence&& srcSequence) const
    {
        return RefFilterSequence<SrcSequence, Filter>(std::move(srcSequence), Filter(filter));
    }

    template <typename SrcSequence,
              std::enable_if_t<!Internal::HasGetNextValueRef<SrcSequence>::value, int> = 0>
    auto create(SrcSequence&& srcSequence) const
    {
        return CopyFilterSequence<SrcSequence, Filter>(std::move(srcSequence), Filter(filter));
    }

private:
    Filter filter;
};
}

template <typename Filter>
auto filter(Filter&& filter)
{
    return Internal::FilterSequenceMutator<Filter>(std::move(filter));
}
}
