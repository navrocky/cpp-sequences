#pragma once

#include <iostream>

namespace Sequences
{

template <typename ResultValueType>
class Sequence
{
public:
    using ValueType = ResultValueType;

//    bool getNextValue(ValueType& v);

//    virtual ~Sequence() {}

//    virtual bool getNextValue(ValueType& v) = 0;
};

//template <typename SrcValueType, typename SequenceCreator>
//auto operator%(Sequence<SrcValueType>& src, const SequenceCreator& creator)
//{
//    return creator.create(src);
//}

template <typename Sequence, typename SequenceCreator>
auto operator%(Sequence&& src, SequenceCreator&& creator)
{
    return creator.create(std::move(src));
}


}
