#pragma once

#include <vector>

namespace Sequences
{

class ToNewVectorContinuation
{
public:
    template <typename SrcSequence>
    std::vector<typename SrcSequence::ValueType> create(SrcSequence&& srcSequence) const
    {
        // TODO made this more optimal if we know source sequence size

        using ValType = typename SrcSequence::ValueType;
        std::vector<ValType> result;
        ValType val;
        while (srcSequence.getNextValue(val))
        {
            result.push_back(val);
        }
        return result;
    }
};

template <typename VectorType>
class ToVectorContinuation
{
public:
    ToVectorContinuation(VectorType& vector)
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
    return ToNewVectorContinuation();
}

template <typename VectorType>
auto toVector(VectorType& vector)
{
    return ToVectorContinuation<VectorType>(vector);
}
}