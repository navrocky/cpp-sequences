#pragma once

#include "../common/sequence.h"

namespace Sequences
{

template <typename Iterable>
class BasicIterableSequence : public Sequence<typename Iterable::value_type>
{
public:
    using ValueType = typename Iterable::value_type;
    using ValueTypeConstRef = const ValueType *;
    using Iterator = typename Iterable::const_iterator;

    bool getNextValueRef(ValueTypeConstRef& v)
    {
        if (iterator_ == endIterator_)
            return false;
        v = &(*iterator_);
        ++iterator_;
        return true;
    }


    bool getNextValue(ValueType& v)
    {
        if (iterator_ == endIterator_)
            return false;
        v = *iterator_;
        ++iterator_;
        return true;
    }

protected:
    Iterator iterator_;
    Iterator endIterator_;
};

template <typename Iterable>
class RefIterableSequence : public BasicIterableSequence<Iterable>
{
public:
    using ValueType = typename Iterable::value_type;
    using Iterator = typename Iterable::const_iterator;

    RefIterableSequence(const Iterable& iterable)
    {
        this->iterator_ = iterable.begin();
        this->endIterator_ = iterable.end();
    }
};

template <typename Iterable>
class CopyIterableSequence : public BasicIterableSequence<Iterable>
{
public:
    CopyIterableSequence(Iterable&& iterable)
        : iterable_(std::move(iterable))
    {
        this->iterator_ = iterable_.begin();
        this->endIterator_ = iterable_.end();
    }

    CopyIterableSequence(CopyIterableSequence<Iterable>&& src)
        : iterable_(std::move(src.iterable_))
    {
        this->iterator_ = std::move(src.iterator_);
        this->endIterator_ = std::move(src.endIterator_);
    }

private:
    Iterable iterable_;
};

template <typename Iterable>
auto sequenceFromIterableRef(const Iterable& iterable)
{
    return RefIterableSequence<Iterable>(iterable);
}

template <typename Iterable>
auto sequenceFromIterableMove(Iterable&& iterable)
{
    return CopyIterableSequence<Iterable>(std::move(iterable));
}

template <typename T>
auto sequenceFromInitializer(const std::initializer_list<T>& l)
{
    return RefIterableSequence<typename std::initializer_list<T>>(l);
}

}
