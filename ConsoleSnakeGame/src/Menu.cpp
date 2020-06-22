#include <iostream>
#include <windows.h>

#include "Menu.h"
#include "defines.h"

Menu::Menu()
    : mState(EMenuState::Navigation)
    , mPlayerChoice(0)
{
    mMenuItems = {
        {0, "Play"},
        {1, "Demo"},
        {2, "Exit"}
    };
}

void Menu::Draw()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, COLOR_WHITE);
    std::cout << "---Menu---\n";
    // Display Menu
    int i = 0;

    for (auto it = mMenuItems.begin(); it != mMenuItems.end() ; ++it)
    {
        if (i == mPlayerChoice)
        {
            SetConsoleTextAttribute(hConsole, COLOR_GREEN);
        }
        else
        {
            SetConsoleTextAttribute(hConsole, COLOR_WHITE);
        }
        std::cout << "  -" << it->second << "-\n";
        ++i;
    }
    std::cout << std::endl;
}

void Menu::Reset()
{
    mPlayerChoice = 0;
    mState = EMenuState::Navigation;
}

void Menu::OnEnter()
{
    switch (mPlayerChoice)
    {
    case 0:
        // start game;
        mState = EMenuState::Play;
        break;
    case 1:
        // demo mode
        mState = EMenuState::Demo;
        break;
    case 2:
        // exit game
        mState = EMenuState::Exit;
        break;
    }
}
