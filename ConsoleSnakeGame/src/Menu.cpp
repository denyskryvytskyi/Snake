#include <iostream>
#include <windows.h>

#include "Menu.h"
#include "defines.h"

Menu::Menu()
    : mState(EMenuState::Navigation)
    , mPlayerChoice(0)
{
    mMenuItems = {
        {"Play", EMenuState::Play},
        {"Demo", EMenuState::Demo},
        {"Exit", EMenuState::Exit}
    };
}

void Menu::Draw()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, COLOR_WHITE);

    std::cout << "---Menu---\n";
    // Display Menu
    int i = 0;
    for (MenuItem item : mMenuItems)
    {
        if (i == mPlayerChoice)
        {
            SetConsoleTextAttribute(hConsole, COLOR_GREEN);
        }
        else
        {
            SetConsoleTextAttribute(hConsole, COLOR_WHITE);
        }
        std::cout << "  -" << item.mName << "-\n";
        ++i;
    }
    std::cout << std::endl;
}

void Menu::Reset(bool hardReset)
{
    if (hardReset)
    {
        mPlayerChoice = 0;
        mState = EMenuState::Navigation;
    }
}

void Menu::OnEnter()
{
    mState = mMenuItems.at(mPlayerChoice).mState;
}
