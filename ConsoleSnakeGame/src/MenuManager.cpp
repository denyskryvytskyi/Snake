#include <windows.h>

#include "MenuManager.h"
#include "defines.h"

MenuManager::MenuManager()
{
    mMenu = new Menu();
}

MenuManager::~MenuManager()
{
    if (mMenu != nullptr)
    {
        delete mMenu;
    }
}

void MenuManager::Render()
{
    mMenu->Draw();
}

void MenuManager::Reset()
{
    mMenu->Reset();
}

void MenuManager::InputHandler()
{
    // menu item choose
    if (GetAsyncKeyState(KEY_ENTER))
    {
        mMenu->OnEnter();
        return;
    }

    // menu item navigation
    int newChoice = mMenu->GetCurrentChoice();
    if (GetAsyncKeyState(KEY_UP) || GetAsyncKeyState(KEY_W))
    {
        --newChoice;
    }
    if (GetAsyncKeyState(KEY_DOWN) || GetAsyncKeyState(KEY_S))
    {
        ++newChoice;
    }

    if (newChoice >= 0 && newChoice < mMenu->GetItemsCount())
    {
        mMenu->SetNewChoice(newChoice);
    }
}
