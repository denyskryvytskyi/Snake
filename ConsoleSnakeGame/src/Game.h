#pragma once

#include "common_types.h"

class GameManager;
class MenuManager;

class Game
{
public:
    enum class EGameState
    {
        BeforePlay,
        Play,
        GameOver,
        Menu,
        Demo,
        Exit,
    };

public:
    Game();
    ~Game();
    //
    void Init();
    //
    void ProcessInput();
    void Update();
    void Render();
    void Reset();
    //
    EGameState GetCurrentState() const { return mState; }

private:
    void setStateByMenuChoice(EMenuState state);

private:
    EGameState mState;

    MenuManager* mMenuManager;
    GameManager* mGameManager;
};

