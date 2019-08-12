// clazy:excludeall=non-pod-global-static

#include "catch.hpp"

#include <iostream>
#include <vector>

#include "../lib/sequences.h"

using namespace std;
using namespace Sequences;

// struct A {
//    using ValueType = int;
//};

// template <typename T>
// struct B {
//    using SrcValueType = typename T::ValueType;
//};

// template <typename Sequence>
// auto createB(Sequence sec)
//{
//    return B<Sequence>();
//}

int id = 0;

class IntVector : public std::vector<int>
{
public:
    IntVector() {}

    IntVector(std::initializer_list<int> l)
        : std::vector<int>(l)
    {
        id_ = ++id;
        std::cout << __FUNCTION__ << id_ << " size: " << size() << std::endl;
    }

    IntVector(IntVector&& src)
        : std::vector<int>(std::move(src))
    {
        id_ = ++id;
        std::cout << __FUNCTION__ << id_ << " size: " << size() << " src_size: " << src.size()
                  << std::endl;
    }

    ~IntVector() { std::cout << __FUNCTION__ << id_ << " size: " << size() << std::endl; }

    IntVector(const IntVector&) = delete;

private:
    int id_ = 0;
};

template <class Sequence>
void printSequence(Sequence& seq)
{
    typename Sequence::ValueType val;
    while (seq.getNextValue(val))
    {
        std::cout << val << endl;
    }
}

TEST_CASE("Iterable sequence")
{

    //    auto v = new IntVector({ 1, 2, 3 });

    //    auto v2 = IntVector(IntVector({ 1, 2, 3 }));

    //    auto sec = sequenceFromIterableMove(*v);
    auto seq = createMapperSequence(sequenceFromIterableMove(IntVector{ 1, 2, 3 }),
                                    [](int i) { return i + 1; });

    printSequence(seq);

    //    MappingSequence<CopyIterableSequence<IntVector>> mapSeq(seq, [](int i){ return i+1;});

    //    delete v;

    //    auto sec2 = sequenceFromIterable(IntVector{ 1, 2, 3 });
    //    //    auto b = createB(sec);

    //        int val;
    //        IntVector dstv;
    //        while (sec.getNextValue(val))
    //        {
    //            dstv.push_back(val);
    //        }

    //        REQUIRE(*v == dstv);
}

TEST_CASE("Mapping sequence")
{
    //    IntVector v = { 1, 2, 3 };
    //    IntVector v2 = { 2, 3, 4 };

    //    auto sequence = sequenceFromIterable(v);

    //    auto factory = map([](int i) { return i + 1; });
    //    IterableSequence<vector<int>> seq(v);

    //    auto mappingSec = factory.create(seq);
    //    int val;
    //    IntVector dstv;
    //    while (mappingSec.getNextValue(val))
    //    {
    //        dstv.push_back(val);
    //    }

    //    REQUIRE(dstv == v2);
}

TEST_CASE("Combining operator")
{
    //    IntVector v = { 1, 2, 3 };
    //    IntVector v2 = { 2, 3, 4 };

    //    //    auto seq1 = sequenceFromIterable(v);
    //    auto seq = sequenceFromIterable(v)
    ////               % map([](int i) { return i + 1; })
    //               % map([](int i) { return i * 2; });

    //    IntVector dstv;
    //    int val;
    //    while (seq.getNextValue(val))
    //    {
    //        dstv.push_back(val);
    //    }
    //    REQUIRE(dstv == v2);
}
