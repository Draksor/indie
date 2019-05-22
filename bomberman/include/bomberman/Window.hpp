#pragma once

#include <irrlicht/irrlicht.h>

#include <indie/log/Logger.hpp>
#include <indie/ecs/System.hpp>

namespace indie::bomberman
{
    class Window : public ecs::System<>
    {
    public:
        Window(const wchar_t *title);

        ~Window() = default;

        virtual void Update() final;

    private:
        static constexpr irr::video::E_DRIVER_TYPE DefaultDriver()
        {
#ifdef _WIN32
            return irr::video::E_DRIVER_TYPE::EDT_OPENGL;
#else
            return irr::video::E_DRIVER_TYPE::EDT_OPENGL;
#endif
        }

    private:
        log::Logger _log{"indie::bomberman::Window"};

        irr::IrrlichtDevice *_dev;
    };
}