#pragma once

#include "../common/sequence.h"
#include <vector>

namespace Sequences
{

class ToNewVectorCollector
{
public:
    template <typename SrcSequence>
    std::vector<typename SrcSequence::ValueType> create(SrcSequence&& srcSequence) const
    {
        // TODO made this more optimal if we know source sequence size

        using ValType = typename SrcSequence::ValueType;
        std::vector<ValType> result;
        ValType val;
        while (Internal::getNextSequenceValue(
            srcSequence, [&result](const ValType& val) { result.push_back(val); }))
        {
        }
        return result;
    }
};

template <typename VectorType>
class ToVectorCollector
{
public:
    ToVectorCollector(VectorType& vector)
        : vector(vector)
    {
    }

    template <typename SrcSequence>
    void create(SrcSequence&& srcSequence) const
    {
        // TODO made this more optimal if we know source sequence size

        using ValType = typename SrcSequence::ValueType;
        ValType val;
        while (srcSequence.getNextValue(val))
        {
            vector.push_back(val);
        }
    }

private:
    VectorType& vector;
};

auto toVector()
{
    return ToNewVectorCollector();
}

template <typename VectorType>
auto toVector(VectorType& vector)
{
    return ToVectorCollector<VectorType>(vector);
}
}
