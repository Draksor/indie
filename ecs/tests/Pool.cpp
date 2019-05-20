#include <gtest/gtest.h>

#include <indie/ecs/Pool.hpp>

#define DEFAULT_MANA_VALUE  200

struct ManaComponent
{
    ManaComponent() = default;
    explicit ManaComponent(int mana) :
        Mana(mana)
    {}

    int Mana{DEFAULT_MANA_VALUE};
};

TEST(ManaComponent, 3Entities)
{
    indie::ecs::Pool<ManaComponent> pool;
    indie::ecs::Entity et1 = 0;
    indie::ecs::Entity et2 = 15;
    indie::ecs::Entity et3 = 8;

    ASSERT_TRUE(pool.Size() == 0);
    ASSERT_TRUE(pool.IsEmpty());

    ASSERT_EQ(pool.Get(et1), nullptr);
    pool.Assign(et1, 450);
    ASSERT_EQ(pool.Size(), 1);
    ASSERT_FALSE(pool.IsEmpty());

    ManaComponent *comp = pool.Get(et1);
    ASSERT_NE(comp, nullptr);
    ASSERT_EQ(comp->Mana, 450);

    pool.Assign(et2, 1743);
    ASSERT_EQ(pool.Size(), 2);
    
    pool.Assign(et3);
    ASSERT_EQ(pool.Size(), 3);
    ManaComponent *comp2 = pool.Get(et2);
    ManaComponent *comp3 = pool.Get(et3);
    ASSERT_NE(comp2, nullptr);
    ASSERT_EQ(comp2->Mana, 1743);
    ASSERT_NE(comp3, nullptr);
    ASSERT_EQ(comp3->Mana, DEFAULT_MANA_VALUE);

    pool.Replace(et1, 666);
    comp = pool.Get(et1);
    ASSERT_EQ(comp->Mana, 666);
    ASSERT_EQ(pool.Size(), 3);
    
    ASSERT_TRUE(pool.Has(et2));
    pool.Delete(et2);
    ASSERT_FALSE(pool.Has(et2));
    comp2 = pool[et2];
    ASSERT_EQ(comp2, nullptr);
    ASSERT_EQ(pool.Size(), 2);

    pool.Replace(et3, 3);
    comp3 = pool.Get(et3);
    ASSERT_EQ(comp3->Mana, 3);
    
    pool.AssignOrReplace(et2);
    comp2 = pool.Get(et2);
    ASSERT_NE(comp2, nullptr);
    ASSERT_EQ(comp2->Mana, 200);
    ASSERT_EQ(pool.Size(), 3);

    pool.AssignOrReplace(et3, 1);
    comp3 = pool.Get(et3);
    ASSERT_NE(comp3, nullptr);
    ASSERT_EQ(comp3->Mana, 1);
    ASSERT_EQ(pool.Size(), 3);


    int pool_index{0};
    pool.ForEach([&](const auto et, const ManaComponent &comp) {
        if (et == et1) {
            ASSERT_EQ(comp.Mana, 666);
        }
        else if (et == et2) {
            ASSERT_EQ(comp.Mana, 200);
        }
        else if (et == et3) {
            ASSERT_EQ(comp.Mana, 1);
        }
        ++pool_index;
    });
    ASSERT_EQ(pool.Size(), pool_index);

    pool.Reset();
    ASSERT_EQ(pool.Size(), 0);
}