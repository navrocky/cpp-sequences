#pragma once

#include <type_traits>

#include "sequence.h"

namespace Sequences
{

template <typename Iterable>
class BasicIterableSequence : public Sequence<typename Iterable::value_type>
{
public:
    using ValueType = typename Iterable::value_type;
    using Iterator = typename Iterable::const_iterator;

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


//template <typename Iterable>
//class RefIterableSequence : public Sequence<typename Iterable::value_type>
//{
//public:
//    using ValueType = typename Iterable::value_type;
//    using Iterator = typename Iterable::const_iterator;

//    RefIterableSequence(const Iterator& begin, const Iterator& end)
//        : iterator(begin)
//        , endIterator(end)
//    {
//        std::cout << __FUNCTION__ << std::endl;
//    }

//    RefIterableSequence(const Iterable& iterable)
//        : iterator(iterable.begin())
//        , endIterator(iterable.end())
//    {
//        std::cout << __FUNCTION__ << std::endl;
//    }

//    ~RefIterableSequence() { std::cout << __FUNCTION__ << std::endl; }

//    bool getNextValue(ValueType& v)
//    {
//        if (iterator == endIterator)
//            return false;
//        v = *iterator;
//        ++iterator;
//        return true;
//    }

//private:
//    Iterator iterator;
//    Iterator endIterator;
//};

template <typename Iterable>
class CopyIterableSequence : public BasicIterableSequence<Iterable>
{
public:
    CopyIterableSequence(Iterable&& iterable)
        : iterable_(std::move(iterable))
    {
        this->iterator_ = iterable_.begin();
        this->endIterator_ = iterable_.end();

        std::cout << __FUNCTION__ << std::endl;
    }

    CopyIterableSequence(CopyIterableSequence<Iterable>&& src)
        : iterable_(std::move(src.iterable_))
    {
        this->iterator_ = std::move(src.iterator_);
        this->endIterator_ = std::move(src.endIterator_);

        std::cout << __FUNCTION__ << std::endl;
    }

    ~CopyIterableSequence() { std::cout << __FUNCTION__ << std::endl; }

private:
    Iterable iterable_;
};

//template <typename Iterable>
//auto sequenceFromIterableRef(const Iterable& iterable)
//{
//    return RefIterableSequence<Iterable>(iterable);
//}

template <typename Iterable>
auto sequenceFromIterableMove(Iterable&& iterable)
{
    return CopyIterableSequence<Iterable>(std::move(iterable));
}
}
