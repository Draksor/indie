#ifdef _WIN32
#endif

#include <indie/log/Logger.hpp>

#include "indie/bomberman/Game.hpp"

int main([[maybe_unused]] int ac, [[maybe_unused]] char **av)
{
    indie::log::Logger log{"bomberman"};

    indie::bomberman::Game game;

    log.Info("Program exited correctly");
}