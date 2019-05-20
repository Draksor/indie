#include <gtest/gtest.h>

#include <indie/ecs/details/SparseSet.hpp>

TEST(UnsignedIntSparseSet, 10Elements)
{
    indie::ecs::details::SparseSet<unsigned int> ss;

    ASSERT_TRUE(ss.IsEmpty());

    ss.Insert(5);
    ss.Insert(34);
    ss.Insert(340);
    ss.Insert(22);
    ss.Insert(13);
    ss.Insert(7);
    ss.Insert(2);
    ss.Insert(0);
    ss.Insert(56);
    ss.Insert(98);

    ASSERT_FALSE(ss.IsEmpty());
    ASSERT_TRUE(ss.Has(7));
    ASSERT_EQ(ss.Size(), 10);

    ss.Erase(7);

    ASSERT_EQ(ss.Size(), 9);
    ASSERT_FALSE(ss.Has(7));

    auto elements_nb = 0;

    for (auto it = ss.Begin(); it != ss.End(); ++it) {
        ASSERT_TRUE(ss.Has(*it));
        if (*it == 98) {
            ss.Erase(it);
        }
        ++elements_nb;
    }
    ASSERT_EQ(elements_nb, ss.Size());
    ASSERT_FALSE(ss.Has(98));

    ASSERT_EQ(ss.Capacity(), 340 + 1);
}