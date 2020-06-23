#pragma once

#include <unordered_map>
#include <string>

#include "common_types.h"

struct MenuItem
{
    friend class Menu;
    
public:
    MenuItem(std::string name, EMenuState state)
        : mName(name)
        , mState(state)
    {}

private:
    std::string mName;
    EMenuState mState;
};

class Menu
{
public:
    Menu();

    void Draw();
    void Reset(bool hardReset);
    //
    int GetCurrentChoice() const { return mPlayerChoice; }
    void SetNewChoice(const int choice) { mPlayerChoice = choice; }
    //
    void OnEnter();
    //
    EMenuState GetState() const { return mState; }
    int GetItemsCount() const { return mMenuItems.size(); }

private:
    std::vector<MenuItem> mMenuItems;

    EMenuState mState;
    int mPlayerChoice;
};

