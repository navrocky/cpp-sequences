#pragma once

#include <type_traits>
#include <utility>

#include "tools.h"

namespace Sequences
{

template <typename ResultValueType>
class Sequence
{
public:
    using ValueType = ResultValueType;
    using ValueTypeConstRef = const ResultValueType*;
};

// template <typename SrcValueType, typename SequenceCreator>
// auto operator%(Sequence<SrcValueType>& src, const SequenceCreator& creator)
//{
//    return creator.create(src);
//}

template <typename Sequence, typename Continuation>
auto operator>>(Sequence&& src, Continuation&& continuation)
{
    return continuation.create(std::move(src));
}

namespace Internal
{

template <typename Sequence, typename Block,
          std::enable_if_t<!HasGetNextValueRef<Sequence>::value, int> = 0>
bool getNextSequenceValue(Sequence& sequence, const Block& block)
{
    typename Sequence::ValueType val;
    if (!sequence.getNextValue(val))
        return false;
    block(val);
    return true;
}

template <typename Sequence, typename Block,
          std::enable_if_t<HasGetNextValueRef<Sequence>::value, int> = 0>
bool getNextSequenceValue(Sequence& sequence, const Block& block)
{
    const typename Sequence::ValueType* val;
    if (!sequence.getNextValueRef(val))
        return false;
    block(*val);
    return true;
}
}
}
