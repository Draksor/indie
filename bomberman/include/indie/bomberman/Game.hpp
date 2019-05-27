#pragma once

#include <memory>

#include <indie/log/Logger.hpp>
#include <indie/ecs/EntityManager.hpp>
#include <indie/ecs/System.hpp>

#include "./Window.hpp"

namespace indie::bomberman
{
    class Game
    {
    public:
        Game();
        ~Game() = default;

    private:
        bool _is_running{true};

        ecs::EntityManager<> _em;
        ecs::SystemManager<> _sm{_em};

        std::shared_ptr<Window> _win;
    };
}