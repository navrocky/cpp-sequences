// clazy:excludeall=non-pod-global-static

#include "catch.hpp"

#include <iostream>
#include <vector>

#include "../lib/sequences.h"

using namespace std;
using namespace Sequences;

//#define PRINT_DEBUG

#ifdef PRINT_DEBUG
static int id = 0;
#endif

class IntVector : public std::vector<int>
{
public:
    IntVector() {}

    IntVector(std::initializer_list<int> l)
        : std::vector<int>(l)
    {
#ifdef PRINT_DEBUG
        id_ = ++id;
        std::cout << __FUNCTION__ << id_ << " size: " << size() << std::endl;
#endif
    }

    IntVector(IntVector&& src)
        : std::vector<int>(std::move(src))
    {
#ifdef PRINT_DEBUG
        id_ = ++id;
        std::cout << __FUNCTION__ << id_ << " size: " << size() << " src_size: " << src.size()
                  << std::endl;
#endif
    }

    ~IntVector()
    {
#ifdef PRINT_DEBUG
        std::cout << __FUNCTION__ << id_ << " size: " << size() << std::endl;
#endif
    }

    IntVector(const IntVector&) = delete;

private:
#ifdef PRINT_DEBUG
    int id_ = 0;
#endif
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
    return MappingSequenceContinuation<Mapper>(std::move(mapper));
}

TEST_CASE("Mapping sequence")
{
    auto seq = sequenceFromIterableMove(IntVector{ 1, 2, 3 }) >> map([](int i) { return i + 1; });
    REQUIRE(collectSequence(seq) == IntVector{ 2, 3, 4 });

    auto factory = createMappingFactory([](int i) { return i + 1; });

    auto seq2 = factory.create(sequenceFromIterableMove(IntVector{ 1, 2, 3 }));
    REQUIRE(collectSequence(seq2) == IntVector{ 2, 3, 4 });
}

TEST_CASE("Range sequence")
{
    auto seq = sequenceFromRangeExcl(1, 5);
    REQUIRE(collectSequence(seq) == IntVector{ 1, 2, 3, 4 });

    cout << "Start" << endl;

    sequenceFromRangeExcl(0, 10) >> forEach([](int i) { cout << i << endl; });

    cout << "End" << endl;
}

TEST_CASE("Combining operator")
{
    auto seq = sequenceFromIterableMove(IntVector{ 1, 2, 3 }) >> map([](int i) { return i + 1; });
    REQUIRE(collectSequence(seq) == IntVector{ 2, 3, 4 });
}

TEST_CASE("Sequence join to string")
{
    REQUIRE(sequenceFromRangeExcl(1, 5) >> joinToStdString() == "1, 2, 3, 4");
}

TEST_CASE("Collect to vector")
{
    REQUIRE(sequenceFromRangeExcl(1, 4) >> toVector() == vector<int>{ 1, 2, 3 });

    vector<int> v;
    sequenceFromRangeExcl(1, 4) >> toVector(v);
    REQUIRE(v == vector<int>{ 1, 2, 3 });
}

TEST_CASE("For each")
{
    int summ = 0;
    IntVector v{ 1, 2, 3 };

    std::cout << "start 1" << std::endl;
    BENCHMARK("Sequence iteration")
    {
        for (int j = 0; j < 1000000000; j++)
        {
            sequenceFromIterableRef(v) >> map([](int i) { return i + 1; })
                >> forEach([&summ](int i) { summ += i; });
        }
    }

    //    REQUIRE(9 == summ);
    std::cout << "summ" << summ << std::endl;
    std::cout << "start 2" << std::endl;

    summ = 0;
    BENCHMARK("Simple loop")
    {
        for (int j = 0; j < 1000000000; j++)
        {
            for (auto it = v.begin(); it != v.end(); ++it)
            {
                summ += *it + 1;
            }
        }
    }

    std::cout << "summ" << summ << std::endl;
    std::cout << "start 3" << std::endl;

    summ = 0;
    BENCHMARK("Range sequence loop")
    {
        for (int j = 0; j < 1000000000; j++)
        {
            sequenceFromRangeExcl(1, 4) >> map([](int i) { return i + 1; })
                >> forEach([&summ](int i) { summ += i; });
        }
    }

    std::cout << "summ" << summ << std::endl;
    std::cout << "end" << std::endl;
    //    REQUIRE(9 == summ);
}
