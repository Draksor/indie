#include "indie/bomberman/Game.hpp"

namespace indie::bomberman
{
    Game::Game()
    {
        _win = _sm.Add<Window>(L"Bomberman !");
        
        while (_is_running) {
            _sm.Update();
        }
    }
}