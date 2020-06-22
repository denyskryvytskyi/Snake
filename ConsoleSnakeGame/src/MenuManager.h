#pragma once

#include "Menu.h"

class MenuManager
{
public:
    MenuManager();
    ~MenuManager();

    void Render();
    void Reset();

    void InputHandler();

    EMenuState GetMenuState() const { return mMenu->GetState(); }


private:
    Menu* mMenu;
};

