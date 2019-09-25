// clazy:excludeall=non-pod-global-static

#include "catch.hpp"

#include <iostream>
#include <vector>

#include "../lib/sequences.h"

using namespace std;
using namespace Sequences;

using IntVector = std::vector<int>;

TEST_CASE("Iterable sequence")
{
    auto seq = sequenceFromIterableMove(IntVector{ 1, 2, 3 });
    REQUIRE(seq >> toVector() == IntVector{ 1, 2, 3 });
}

template <typename Mapper>
auto createMappingFactory(Mapper&& mapper)
{
    return MappingSequenceMutator<Mapper>(std::move(mapper));
}

TEST_CASE("Mapping sequence")
{
    auto seq = sequenceFromIterableMove(IntVector{ 1, 2, 3 }) >> map([](int i) { return i + 1; });
    REQUIRE(seq >> toVector() == IntVector{ 2, 3, 4 });

    auto factory = createMappingFactory([](int i) { return i + 1; });

    auto seq2 = factory.create(sequenceFromIterableMove(IntVector{ 1, 2, 3 }));
    REQUIRE(seq2 >> toVector() == IntVector{ 2, 3, 4 });
}

TEST_CASE("Range sequence")
{
    auto seq = sequenceFromRangeExcl(1, 5);
    REQUIRE(seq >> toVector() == IntVector{ 1, 2, 3, 4 });
}

TEST_CASE("Combining operator")
{
    auto seq = sequenceFromIterableMove(IntVector{ 1, 2, 3 }) >> map([](int i) { return i + 1; });
    REQUIRE(seq >> toVector() == IntVector{ 2, 3, 4 });
}

TEST_CASE("Sequence join to string")
{
    REQUIRE(sequenceFromRangeExcl(1, 5) >> joinToStdString() == "1, 2, 3, 4");
    REQUIRE(sequenceFromRangeExcl(1, 5) >> joinToStdString() == "1, 2, 3, 4");
}

TEST_CASE("Collect to vector")
{
    REQUIRE(sequenceFromRangeExcl(1, 4) >> toVector() == vector<int>{ 1, 2, 3 });

    vector<int> v;
    sequenceFromRangeIncl(1, 4) >> toVector(v);
    REQUIRE(v == vector<int>{ 1, 2, 3, 4 });
}

TEST_CASE("For each")
{
    int summ = 0;
    IntVector v{ 1, 2, 3 };

    sequenceFromIterableRef(v) >> map([](int i) { return i + 1; })
        >> forEach([&summ](int i) { summ += i; });

    REQUIRE(9 == summ);
}

TEST_CASE("Sequence from initializer")
{
    int summ = 0;
    sequenceFromInitializer({1, 2, 3}) >> map([](int i) { return i + 1; })
        >> forEach([&summ](int i) { summ += i; });
    REQUIRE(9 == summ);
}

