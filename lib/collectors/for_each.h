#pragma once

#include <utility>

namespace Sequences
{

template <typename Block>
class ForEachContinuation
{
public:
    ForEachContinuation(Block&& block)
        : block(std::move(block))
    {
    }

    ForEachContinuation(const ForEachContinuation<Block>& src) = delete;

    ForEachContinuation(ForEachContinuation<Block>&& src)
        : block(std::move(src.block))
    {
    }

    template <typename SrcSequence>
    void create(SrcSequence&& srcSequence) const
    {
        typename SrcSequence::ValueType val;
        while (srcSequence.getNextValue(val))
        {
            block(val);
        }
    }

private:
    Block block;
};

template <typename Block>
auto forEach(Block&& block)
{
    return ForEachContinuation<Block>(std::move(block));
}
}
