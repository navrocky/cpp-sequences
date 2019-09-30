#pragma once

#include <utility>
#include "../common/sequence.h"

namespace Sequences
{

template <typename Block>
class ForEachCollector
{
public:
    ForEachCollector(Block&& block)
        : block(std::move(block))
    {
    }

    ForEachCollector(const ForEachCollector<Block>& src) = delete;

    ForEachCollector(ForEachCollector<Block>&& src)
        : block(std::move(src.block))
    {
    }

    template <typename SrcSequence>
    void create(SrcSequence&& srcSequence) const
    {
        typename SrcSequence::ValueType val;
        while (Internal::getNextSequenceValue(srcSequence, block))
        {
        }
    }

private:
    Block block;
};

template <typename Block>
auto forEach(Block&& block)
{
    return ForEachCollector<Block>(std::move(block));
}
}
