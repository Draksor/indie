#pragma once

#include <type_traits>

namespace indie::meta
{
    /**
     * @brief Compile-time container of types.
     * 
     * @tparam Types Types to store.
     */
    template <typename ...Types>
    struct TypeList
    {
        /**
         * @brief Get the number of types stored.
         * 
         */
        typedef std::integral_constant<uint16_t, sizeof...(Types)> Size;
    };
    /**
     * @brief Creates a new type list from two type lists.
     * 
     * @tparam A First type list.
     * @tparam B Second type list.
     */
    template <typename ...A, typename ...B>
    struct TypeList<TypeList<A...>, TypeList<B...>> : TypeList<A..., B...>
    {};

    /**
     * @brief Tells if a type list contains a type.
     * 
     * @tparam T Type to search.
     * @tparam TypeList Type of the type list to search in.
     */
    template <typename T, typename TypeList>
    struct TypeListHas;
    template <typename T>
    struct TypeListHas<T, TypeList<>> : std::false_type
    {};
    template <typename T, typename ...Types>
    struct TypeListHas<T, TypeList<T, Types...>> : std::true_type
    {};
    template <typename T, typename U, typename ...Types>
    struct TypeListHas<T, TypeList<U, Types...>> : TypeListHas<T, TypeList<Types...>>
    {};

    /**
     * @brief Concatenates a type list with a type in a new list and returns it.
     * 
     * @tparam T Type to append.
     * @tparam TypeList Type of the type list
     */
    template <typename T, typename TypeList>
    struct TypeListCat;
    template <typename T, typename ...Types>
    struct TypeListCat<T, TypeList<Types...>>
    {
        using Type = TypeList<Types..., T>;
    };
    /**
     * @brief Concatenates a type list with another type list in a new list and returns it.
     * 
     * @tparam A First type list.
     * @tparam B Second type list.
     */
    template <typename ...A, typename ...B>
    struct TypeListCat<TypeList<A...>, TypeList<B...>>
    {
        using Type = TypeList<A..., B...>;
    };
}