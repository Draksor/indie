#include <gtest/gtest.h>

#include <indie/ecs/EntityManager.hpp>

struct Stamina
{
    Stamina() = default;
    explicit Stamina(int st) : Value(st) {}
    int Value{100};
};

struct Mana
{
    Mana() = default;
    explicit Mana(int m) : Value(m) {}
    int Value{200};
};

TEST(EntityRegistry, 4Entities)
{
    indie::ecs::EntityManager<unsigned> reg{};

    auto et = reg.Create();
    
    reg.Create();
    
    auto et3 = reg.Create();
    reg.Destroy(et3);
    ASSERT_EQ(reg.Size(), 2);

    et3 = reg.Create();
    ASSERT_EQ(et3, 2);
    ASSERT_EQ(reg.Size(), 3);
    
    auto et4 = reg.Create();
    ASSERT_EQ(et4, 3);
    ASSERT_EQ(reg.Size(), 4);

    reg.Assign<Stamina>(et);
    ASSERT_TRUE(reg.Has<Stamina>(et));
    ASSERT_EQ(reg.Size<Stamina>(), 1);

    reg.Assign<Mana>(et);
    auto result = reg.Has<Stamina, Mana>(et);
    ASSERT_TRUE(result);

    reg.Assign<Stamina>(et3);
    reg.Assign<Mana>(et3, 589);
    reg.Assign<Mana>(et4);
    reg.Assign<Stamina>(et4, 85948);
    ASSERT_EQ((reg.Size<Stamina, Mana>()), 3);
    ASSERT_EQ((reg.Get<Mana>(et3)->Value), 589);
    auto et3_cmps = reg.Get<Mana, Stamina>(et3);
    ASSERT_EQ((std::get<Mana *>(et3_cmps)->Value), 589);
    ASSERT_EQ((std::get<Stamina *>(et3_cmps)->Value), 100);


    auto counter{0};
    reg.ForEach<Stamina, Mana>([&](const auto et, const Stamina &stamina, const Mana &mana) {
        if (et == et4) {
            ASSERT_EQ(stamina.Value, 85948);
        }
        else if (et == et3) {
            ASSERT_EQ(mana.Value, 589);
        }
        counter++;
    });
    ASSERT_EQ(counter, (reg.Size<Stamina, Mana>()));

    reg.Delete<Stamina, Mana>(et);
    ASSERT_FALSE((reg.Has<Stamina, Mana>(et)));
    ASSERT_EQ((reg.Size<Stamina, Mana>()), 2);

    reg.Destroy(et, et4);
    ASSERT_EQ(reg.Size(), 2);
    result = reg.Size<Stamina, Mana>();
    ASSERT_EQ(result, 1);
    result = reg.Size<Stamina>();
    ASSERT_EQ(result, 1);
    result = reg.Size<Mana>();
    ASSERT_EQ(result, 1);

    reg.Reset<Stamina, Mana>();
    ASSERT_EQ((reg.Size<Stamina>()), 0);
    ASSERT_EQ((reg.Size<Mana>()), 0);

    ASSERT_EQ((reg.Size()), 2);
    
    reg.Reset();
    ASSERT_EQ((reg.Size()), 0);
}