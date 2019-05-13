#include <gtest/gtest.h>

#include <indie/meta/Tuple.hpp>

struct Hp {};
struct Stamina {};

TEST(Tuples, 3Types)
{
    using MyTuple = indie::meta::Tuple<Hp, Stamina>;

    ASSERT_EQ(MyTuple::Size(), 2);
    
    bool result = indie::meta::TupleHas<Stamina, MyTuple>();
    ASSERT_TRUE(result);

    result = indie::meta::TupleHas<Hp, MyTuple>();
    ASSERT_TRUE(result);

    result = indie::meta::TupleHas<int, MyTuple>();
    ASSERT_FALSE(result);

    using MyNewTuple = indie::meta::TupleCat<int, MyTuple>::Type;

    ASSERT_EQ(MyNewTuple::Size(), 3);

    result = indie::meta::TupleHas<Stamina, MyNewTuple>();
    ASSERT_TRUE(result);

    result = indie::meta::TupleHas<Hp, MyNewTuple>();
    ASSERT_TRUE(result);

    result = indie::meta::TupleHas<int, MyNewTuple>();
    ASSERT_TRUE(result);

    using MergedTuple = indie::meta::TupleCat<MyTuple, MyNewTuple>::Type;

    ASSERT_EQ(MergedTuple::Size(), 5);

    result = indie::meta::TupleHas<Stamina, MergedTuple>();
    ASSERT_TRUE(result);

    result = indie::meta::TupleHas<Hp, MergedTuple>();
    ASSERT_TRUE(result);

    result = indie::meta::TupleHas<int, MergedTuple>();
    ASSERT_TRUE(result);
}