#pragma once

#include "common_types.h"

class GameManager;
class MenuManager;
class GameParams;

class Game
{
public:
    enum class EGameState
    {
        BeforePlay,
        Play,
        GameOver,
        Menu,
        BackToMenu,
        Demo,
        Exit,
    };

public:
    Game();
    ~Game();
    //
    void Start();
    void Stop();
    //
    void Init();
    //
    void ProcessInput();
    void Update(float dt);
    void Render();
    void Reset();
    //
    EGameState GetCurrentState() const { return mState; }
    bool IsRunning() const { return mRunning; }

private:
    void SetStateByMenuChoice(EMenuState state);
    void CleanConsole();

private:
    EGameState mState;
    //
    MenuManager* mMenuManager;
    GameManager* mGameManager;
    //
    bool mRunning;
    bool mIsForceCls;
};

