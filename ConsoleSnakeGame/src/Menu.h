#pragma once

#include <unordered_map>
#include <string>

#include "common_types.h"

class Menu
{
public:
    Menu();

    void Draw();
    void Reset();
    //
    int GetCurrentChoice() const { return mPlayerChoice; }
    void SetNewChoice(const int choice) { mPlayerChoice = choice; }
    //
    void OnEnter();
    //
    EMenuState GetState() const { return mState; }
    int GetItemsCount() const { return mMenuItems.size(); }

private:
    std::unordered_map<int, std::string> mMenuItems;

    EMenuState mState;
    int mPlayerChoice;
};

