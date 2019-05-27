#pragma once

namespace indie::server
{
    struct KeepAlive
    {

    };

    struct Auth
    {
        char PlayerName[20];
    };

    struct Walk
    {
        int X;
        int Y;
    };
}