#pragma once

namespace Sequences
{

template <typename ValueType>
class Sequence
{
public:
    virtual bool getNextValue(ValueType& v) = 0;
};

template <typename SrcValueType, typename DstSequence, typename SequenceCreator>
DstSequence operator%(const Sequence<SrcValueType>& src, const SequenceCreator& creator)
{

}

}
