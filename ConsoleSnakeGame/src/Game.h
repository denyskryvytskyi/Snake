#pragma once

#include "GameManager.h"

class Game
{
public:
    enum class EGameState
    {
        Active,
        GameOver,
        Menu,
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
    //
    EGameState GetCurrentState() const { return mState; }

private:
    EGameState mState;
    GameManager* mGameManager;
};

