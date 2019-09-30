// clazy:excludeall=non-pod-global-static

#include "catch.hpp"

#include <iostream>
#include <map>
#include <vector>

#include "../lib/sequences.h"

// using namespace std;
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
    return Internal::MappingSequenceMutator<Mapper>(std::move(mapper));
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
    REQUIRE(sequenceFromRangeExcl(1, 4) >> toVector() == IntVector{ 1, 2, 3 });

    IntVector v;
    sequenceFromRangeIncl(1, 4) >> toVector(v);
    REQUIRE(v == IntVector{ 1, 2, 3, 4 });
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
    sequenceFromInitializer({ 1, 2, 3 }) >> map([](int i) { return i + 1; })
        >> forEach([&summ](int i) { summ += i; });
    REQUIRE(9 == summ);
}

TEST_CASE("Sequence generator")
{
    int i = 0;
    auto s = sequenceGenerator([&i](int& v) {
                 v = i++;
                 return v < 10;
             })
             >> joinToStdString(",");

    REQUIRE(s == "0,1,2,3,4,5,6,7,8,9");
}

TEST_CASE("Sequence from std::map")
{
    using Map = std::map<int, std::string>;
    Map m;
    m[0] = "zero";
    m[1] = "one";

    auto s = sequenceFromIterableRef(m)
             >> map([](const Map::value_type& pair) { return pair.second; })
             >> joinToStdString(",");

    REQUIRE(s == "zero,one");
}

TEST_CASE("Check for generic getNextSequenceValue")
{
    auto seq1 = sequenceGenerator([](int& v) {
        v = 1;
        return true;
    });
    auto seq2 = sequenceFromInitializer({ 1, 2, 3 });

    Internal::getNextSequenceValue(seq1, [](int v) {
        REQUIRE(v == 1);
        return true;
    });

    Internal::getNextSequenceValue(seq2, [](int v) {
        REQUIRE(v == 1);
        return true;
    });
}

TEST_CASE("Test filtering")
{
    auto s = sequenceFromInitializer({ 1, 2, 3, 4 }) >> filter([](int i) { return i % 2 == 0; })
             >> joinToStdString(",");
    REQUIRE(s == "2,4");

    int i = 0;
    auto s2 = sequenceGenerator([&i](std::string& v) {
                  v = std::to_string(++i);
                  return i < 5;
              })
              >> filter([](const std::string& s) { return s == "1" || s == "3"; })
              >> joinToStdString(",");

    REQUIRE(s2 == "1,3");
}

TEST_CASE("All")
{
    int i = 0;
    auto s = sequenceGenerator([&i](int& v) {
        v = i++;
        return v < 10;
    }) >> filter([](int i) { return i % 2 == 0; })
        >> map([](int i) { return std::to_string(i) + "v"; }) >> joinToStdString(",");

    REQUIRE(s == "0v,2v,4v,6v,8v");
}

TEST_CASE("Test first or default")
{
    auto v = sequenceFromInitializer({ 1, 2, 3, 4 }) >> firstOrDefault(0);
    REQUIRE(v == 1);

    auto v2 = sequenceFromInitializer<int>({}) >> firstOrDefault(10);
    REQUIRE(v2 == 10);

}
