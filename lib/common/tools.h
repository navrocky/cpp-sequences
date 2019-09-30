#pragma once

#include <type_traits>

namespace Sequences
{

namespace Internal
{

template <typename T>
struct HasGetNextValueRef
{
private:
    using ValueType = typename T::ValueType;
    using ValueTypeConstRef = const ValueType*;

    template <typename C>
    static constexpr auto check(C*) -> typename std::is_same<
        decltype(std::declval<C>().getNextValueRef(std::declval<ValueTypeConstRef&>())),
        bool>::type;

    template <typename>
    static constexpr std::false_type check(...);

    typedef decltype(check<T>(0)) type;

public:
    static constexpr bool value = type::value;
};
}
}
