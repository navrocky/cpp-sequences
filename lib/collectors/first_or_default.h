#pragma once

#include "../common/sequence.h"
#include <utility>

namespace Sequences
{

template <typename ValueType>
class FirstOrDefaultCollector
{
public:
    FirstOrDefaultCollector(const ValueType& defaultValue)
        : defaultValue(defaultValue)
    {
    }

    FirstOrDefaultCollector(const FirstOrDefaultCollector<ValueType>& src) = delete;

    FirstOrDefaultCollector(FirstOrDefaultCollector<ValueType>&& src)
        : defaultValue(std::move(src.defaultValue))
    {
    }

    template <typename SrcSequence>
    ValueType create(SrcSequence&& srcSequence) const
    {
        typename SrcSequence::ValueType result;
        if (Internal::getNextSequenceValue(
                srcSequence,
                [&result](const typename SrcSequence::ValueType& val) { result = val; }))
            return result;
        else
            return defaultValue;
    }

private:
    ValueType defaultValue;
};

template <typename T>
FirstOrDefaultCollector<T> firstOrDefault(const T& defaultValue)
{
    return FirstOrDefaultCollector<T>(defaultValue);
}
}
