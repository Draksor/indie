#include "bomberman/Window.hpp"

namespace indie::bomberman
{
    Window::Window(const wchar_t *title) :
        _dev{irr::createDevice(DefaultDriver(), irr::core::dimension2d<irr::u32>(1920, 1080), 16, true, false, false, 0)}
    {
        if (!_dev) {
            _log.Critical("Failed to create video device.");
             std::exit(1);
        }
        _dev->setWindowCaption(title);
    }

    void Window::Update()
    {
        if (_dev->run()) {
        }
    }
}