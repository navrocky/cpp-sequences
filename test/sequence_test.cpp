// clazy:excludeall=non-pod-global-static

#include "catch.hpp"

#include <iostream>
#include <vector>

#include "../lib/sequences.h"

using namespace std;
using namespace Sequences;


static int id = 0;

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

template <class Sequence>
vector<typename Sequence::ValueType> collectSequence(Sequence& seq)
{
    typename Sequence::ValueType val;
    vector<typename Sequence::ValueType> result;
    while (seq.getNextValue(val))
    {
        result.push_back(val);
    }
    return result;
}

TEST_CASE("Iterable sequence")
{
    auto seq = sequenceFromIterableMove(IntVector{ 1, 2, 3 });
    REQUIRE(collectSequence(seq) == IntVector{ 1, 2, 3 });
}

template <typename Mapper>
auto createMappingFactory(Mapper&& mapper)
{
    return MappingSequenceFactory<Mapper>(std::move(mapper));
}

TEST_CASE("Mapping sequence")
{
    auto seq = createMappingSequence(sequenceFromIterableMove(IntVector{ 1, 2, 3 }),
                                     [](int i) { return i + 1; });
    REQUIRE(collectSequence(seq) == IntVector{ 2, 3, 4 });

    auto factory = createMappingFactory([](int i) { return i + 1; });

    auto seq2 = factory.create(sequenceFromIterableMove(IntVector{ 1, 2, 3 }));
    REQUIRE(collectSequence(seq2) == IntVector{ 2, 3, 4 });
}

TEST_CASE("Combining operator")
{
    auto seq = sequenceFromIterableMove(IntVector{ 1, 2, 3 }) % "asdas";//map([](int i) { return i + 1; });
    REQUIRE(collectSequence(seq) == IntVector{ 2, 3, 4 });

}
