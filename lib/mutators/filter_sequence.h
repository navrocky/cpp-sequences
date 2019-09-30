#pragma once
#include "../common/sequence.h"

namespace Sequences
{

template <typename SrcSequence, typename Filter>
class FilterSequence : public Sequence<typename SrcSequence::ValueType>
{
public:
    using ValueType = typename SrcSequence::ValueType;
    using ValueTypeConstRef = const ValueType*;

    FilterSequence(SrcSequence&& srcSequence, Filter&& mapper)
        : srcSequence(std::move(srcSequence))
        , filter(std::move(mapper))
    {
    }

    FilterSequence(FilterSequence<SrcSequence, Filter>&& src)
        : srcSequence(std::move(src.srcSequence))
        , filter(std::move(src.filter))
    {
    }

    bool getNextValueRef(ValueTypeConstRef& v)
    {
        bool found = false;
        while (Internal::getNextSequenceValue(srcSequence,
                                              [&v, &found, this](const ValueType& srcVal) {
                                                  found = filter(srcVal);
                                                  if (found)
                                                      v = srcVal;

                                              }))
        {
            if (found)
                return true;
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

    template <typename SrcSequence>
    FilterSequence<SrcSequence, Filter> create(SrcSequence&& srcSequence) const
    {
        return FilterSequence<SrcSequence, Filter>(std::move(srcSequence), Filter(filter));
    }

private:
    Filter filter;
};

template <typename Filter>
auto filter(Filter&& filter)
{
    return FilterSequenceMutator<Filter>(std::move(filter));
}
}
