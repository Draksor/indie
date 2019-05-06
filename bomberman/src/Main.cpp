#include <indie/log/Logger.hpp>

int main([[maybe_unused]] int ac, [[maybe_unused]] char **av)
{
    indie::log::Logger log{"bomberman"};

    log.Info("Program exited correctly");
}