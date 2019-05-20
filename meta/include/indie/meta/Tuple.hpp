#pragma once

#include <tuple>

#include "TypeList.hpp"

namespace indie::meta
{
    template <typename ...Types>
    struct Tuple : std::tuple<Types...>
    {
        using std::tuple<Types...>::tuple;
        typedef std::integral_constant<uint16_t, sizeof...(Types)> Size;

        template <typename Type>
        const Type &Get() const noexcept
        {
            return std::get<Type>(*this);
        }
        template <typename Type>
        Type &Get() noexcept
        {
            return const_cast<Type &>(std::as_const(*this).template Get<Type>());
        }
    };
    template <typename ...Types>
    struct Tuple<TypeList<Types...>> : Tuple<Types...>
    {};

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

    template <typename T, typename Tuple>
    struct TupleIndex;
    template <typename T, typename ...Types>
    struct TupleIndex<T, Tuple<T, Types...>>
    {
        static constexpr uint16_t Value = 0;
    };
    template <typename T, typename U, typename ...Types>
    struct TupleIndex<T, Tuple<U, Types...>>
    {
        static constexpr uint16_t Value = 1 + TupleIndex<T, Tuple<Types...>>::Value;
    };
}