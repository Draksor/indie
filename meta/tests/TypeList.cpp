#include <gtest/gtest.h>

#include <indie/meta/TypeList.hpp>

struct Hp {};
struct Stamina {};

TEST(TypeLists, 3Types)
{
    using MyList = indie::meta::TypeList<Hp, Stamina>;

    ASSERT_EQ(MyList::Size(), 2);
    
    bool result = indie::meta::TypeListHas<Stamina, MyList>();
    ASSERT_TRUE(result);

    result = indie::meta::TypeListHas<Hp, MyList>();
    ASSERT_TRUE(result);

    result = indie::meta::TypeListHas<int, MyList>();
    ASSERT_FALSE(result);

    using MyNewList = indie::meta::TypeListCat<int, MyList>::Type;

    ASSERT_EQ(MyNewList::Size(), 3);

    result = indie::meta::TypeListHas<Stamina, MyNewList>();
    ASSERT_TRUE(result);

    result = indie::meta::TypeListHas<Hp, MyNewList>();
    ASSERT_TRUE(result);

    result = indie::meta::TypeListHas<int, MyNewList>();
    ASSERT_TRUE(result);

    using MergedList = indie::meta::TypeListCat<MyList, MyNewList>::Type;

    ASSERT_EQ(MergedList::Size(), 5);

    result = indie::meta::TypeListHas<Stamina, MergedList>();
    ASSERT_TRUE(result);

    result = indie::meta::TypeListHas<Hp, MergedList>();
    ASSERT_TRUE(result);

    result = indie::meta::TypeListHas<int, MergedList>();
    ASSERT_TRUE(result);
}