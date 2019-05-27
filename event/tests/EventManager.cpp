#include <gtest/gtest.h>

#include <indie/event/EventManager.hpp>

struct HealEventArgs
{
    int Value{100};
};

struct Player
{
    void OnReceive(HealEventArgs &args)
    {
        Hp += args.Value;
    }

    int Hp{100};
};

TEST(OneHandler, OneObserver)
{
    indie::event::EventManager em;
    Player p;
    HealEventArgs args;

    em.Register<HealEventArgs>(p);
    em.Notify<HealEventArgs>(args);
    ASSERT_EQ(p.Hp, 200);
    ASSERT_TRUE((em.Has<HealEventArgs>(p)));

    em.Notify();
    ASSERT_EQ(p.Hp, 300);

    em.Unregister(p);
    em.Notify();
    ASSERT_EQ(p.Hp, 300);
}