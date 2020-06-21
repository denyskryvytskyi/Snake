#pragma once

#include <windows.h>

#include "Game.h"

bool ShowConsoleCursor(bool show);

int main()
{
    ShowConsoleCursor(false);

    Game game;
    game.Init();

    while (game.GetCurrentState() != Game::EGameState::GameOver)
    {
        game.ProcessInput();

        game.Render();

        game.Update();
    }

    system("pause");
    return 0;
}

bool ShowConsoleCursor(bool show)
{
    CONSOLE_CURSOR_INFO cci;
    HANDLE hStdOut;
    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE)
        return FALSE;
    if (!GetConsoleCursorInfo(hStdOut, &cci))
        return FALSE;
    cci.bVisible = show;
    if (!SetConsoleCursorInfo(hStdOut, &cci))
        return FALSE;
    return TRUE;
}