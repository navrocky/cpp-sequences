#pragma once

#include <utility>

namespace Sequences
{

template <typename ResultValueType>
class Sequence
{
public:
    using ValueType = ResultValueType;
    using ValueTypeConstRef = const ResultValueType*;
};

//template <typename SrcValueType, typename SequenceCreator>
//auto operator%(Sequence<SrcValueType>& src, const SequenceCreator& creator)
//{
//    return creator.create(src);
//}

template <typename Sequence, typename Continuation>
auto operator>>(Sequence&& src, Continuation&& continuation)
{
    return continuation.create(std::move(src));
}

namespace Internal {

//template <typename Sequence, >
//bool getNextSequenceValue(typename Sequence::ValueType& v)
//{

//}

}


}
