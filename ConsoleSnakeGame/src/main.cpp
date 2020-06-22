#pragma once

#include <windows.h>

#include "Game.h"

bool ShowConsoleCursor(bool show);

int main()
{
    ShowConsoleCursor(false);

    Game game;
    game.Start();

    return 0;
}

bool ShowConsoleCursor(bool show)
{
    CONSOLE_CURSOR_INFO cci;
    HANDLE hStdOut;
    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE)
        return false;
    if (!GetConsoleCursorInfo(hStdOut, &cci))
        return false;
    cci.bVisible = show;
    if (!SetConsoleCursorInfo(hStdOut, &cci))
        return false;

    return true;
}