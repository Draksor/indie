#pragma once

#include <tuple>

#include "TypeList.hpp"

namespace indie::meta
{
    template <typename ...Types>
    struct Tuple
    {
        using Type = std::tuple<Types...>;
        typedef std::integral_constant<uint16_t, sizeof...(Types)> Size;
    };
    template <typename ...Types>
    struct Tuple<TypeList<Types...>> : Tuple<Types...>
    {
    };

    template <typename T, typename Tuple>
    struct TupleHas;
    template <typename T>
    struct TupleHas<T, Tuple<>> : std::false_type
    {};
    template <typename T, typename ...Types>
    struct TupleHas<T, Tuple<T, Types...>> : std::true_type
    {};
    template <typename T, typename U, typename ...Types>
    struct TupleHas<T, Tuple<U, Types...>> : TupleHas<T, Tuple<Types...>>
    {};
    template <typename T>
    struct TupleHas<T, std::tuple<>> : std::false_type
    {};
    template <typename T, typename ...Types>
    struct TupleHas<T, std::tuple<T, Types...>> : std::true_type
    {};
    template <typename T, typename U, typename ...Types>
    struct TupleHas<T, std::tuple<U, Types...>> : TupleHas<T, std::tuple<Types...>>
    {};

    template <typename T, typename Tuple>
    struct TupleCat;
    template <typename T, typename ...Types>
    struct TupleCat<T, Tuple<Types...>>
    {
        using Type = Tuple<Types..., T>;
    };
    template <typename ...A, typename ...B>
    struct TupleCat<Tuple<A...>, Tuple<B...>>
    {
        using Type = Tuple<A..., B...>;
    };
}