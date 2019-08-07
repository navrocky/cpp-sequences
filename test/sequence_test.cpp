// clazy:excludeall=non-pod-global-static

#include "catch.hpp"

#include <vector>
#include <type_traits>

#include "../lib/sequences.h"

using namespace std;
using namespace Sequences;

template <typename T>
auto testMap(const T& t)
{
    using ResultType = decltype (t());
    vector<ResultType> v;
    v.push_back(t());
    return v;
}


TEST_CASE("Sequence test")
{
    vector<int> v = { 1, 2, 3 };

//    vector<int> i = testMap([](){ return 10; });

    auto factory = map([](int i){
        return i+1;
    });

//    auto m = map([](int v) { return v + 1; });

    //    auto s = sequenceFromIterable(v) % map() % toQVector();
}
