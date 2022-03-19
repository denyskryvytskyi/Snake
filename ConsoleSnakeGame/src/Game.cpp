#include <windows.h>
#include <iostream>
#include <thread>

#include "Game.h"
#include "GameManager.h"
#include "MenuManager.h"
#include "Timer.h"
#include "defines.h"

Game::Game()
    : mState(EGameState::Menu)
    , mRunning(false)
    , mIsForceCls(false)
{
    mMenuManager = new MenuManager();
    mGameManager = new GameManager();
}

Game::~Game()
{
    if (mMenuManager != nullptr)
    {
        delete mMenuManager;
    }
    if (mGameManager != nullptr)
    {
        delete mGameManager;
    }
}

void Game::Start()
{
    Init();

    Timer timer;
    while (mRunning)
    {
        ProcessInput();

        float elapsedTime = timer.Elapsed();
        timer.Restart();

        Update(elapsedTime);
        Render();
    }
}

void Game::Stop()
{
    mRunning = false;
}

void Game::Init()
{
    mGameManager->Init();
    mRunning = true;
}

void Game::ProcessInput()
{
    if (mState == EGameState::Menu)
    {
        mMenuManager->InputHandler();
        // delay
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    else if (mState == EGameState::Play)
    {
        mGameManager->InputHandler();
    }
    else if (mState == EGameState::GameOver)
    {
        if (GetAsyncKeyState(KEY_ENTER))
        {
            mIsForceCls = true;
            mState = EGameState::BeforePlay;
        }
        else if (GetAsyncKeyState(KEY_BACKSPACE))
        {
            mIsForceCls = true;
            mState = EGameState::BackToMenu;
        }
    }
}

void Game::Update(float dt)
{
    if (mState == EGameState::Menu)
    {
        EMenuState menuState = mMenuManager->GetMenuState();
        // if menu state isn't "Navigation", then change game state
        SetStateByMenuChoice(menuState);
    }
    else if (mState == EGameState::BeforePlay || mState == EGameState::Demo)
    {
        // Reset if it's not a first game in session
        Reset();

        // Before game initializations
        mGameManager->OnStart(mState == EGameState::Demo || mMenuManager->GetMenuState() == EMenuState::Demo);
        mState = EGameState::Play;
    }
    else if (mState == EGameState::Play)
    {
        mGameManager->Update(dt);

        if (!mGameManager->IsSnakeAlive())
        {
            mIsForceCls = true;
            mState = EGameState::GameOver;
        }
    }
    else if (mState == EGameState::BackToMenu)
    {
        mMenuManager->Reset(true);
        mState = EGameState::Menu;
    }
    else if (mState == EGameState::Exit)
    {
        Stop();
    }
}

void Game::Render()
{
    CleanConsole();

    if (mState == EGameState::Menu)
    {
        mMenuManager->Render();
    }
    else if (mState == EGameState::Play)
    {
        mGameManager->Render();
    }
    else if (mState == EGameState::GameOver)
    {
        // simple menu after game
        std::cout << "Game Over!" << std::endl;
        std::cout << "Score: " << mGameManager->GetScore() << std::endl;
        std::cout << "Rank: " << mGameManager->GetRank() << std::endl;
        std::cout << "Click ENTER to restart Game." << std::endl;
        std::cout << "Click BACKSPACE to return in Menu." << std::endl;
    }
}

void Game::Reset()
{
    mGameManager->Reset(true);
    mMenuManager->Reset();
}

void Game::SetStateByMenuChoice(EMenuState state)
{
    switch (state)
    {
    case EMenuState::Play:
        mState = EGameState::BeforePlay;
        break;
    case EMenuState::Demo:
        mState = EGameState::Demo;
        break;
    case EMenuState::Exit:
        mState = EGameState::Exit;
        break;
    }
}

void Game::CleanConsole()
{
    HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD cd;
    cd.X = 0;
    cd.Y = 0;
    SetConsoleCursorPosition(hd, cd);

    if (mIsForceCls)
    {
        system("cls");
        mIsForceCls = false;
    }
}
