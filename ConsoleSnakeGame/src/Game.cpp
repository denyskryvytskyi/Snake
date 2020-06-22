#include <windows.h>
#include <iostream>

#include "Game.h"
#include "GameManager.h"
#include "MenuManager.h"
#include "defines.h"
#include "Semaphore.h"

Semaphore gUpdateSemaphore(1);
Semaphore gRenderSemaphore(0);

Game::Game()
    : mState(EGameState::Menu)
    , mRunning(false)
    , updateThread(&Game::Update, this)
    , renderThread(&Game::Render, this)
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

    updateThread.join();
    renderThread.join();
}

void Game::Start()
{
    Init();
    mRunning = true;
    ProcessInput();
}

void Game::Stop()
{
    mRunning = false;
}

void Game::Init()
{
    mGameManager->Init();
}

void Game::ProcessInput()
{
    while (mRunning)
    {
        // key input handling
        if (mState == EGameState::Menu)
        {
            mMenuManager->InputHandler();
            // delay
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        else if (mState == EGameState::Play)
        {
            mGameManager->InputHandler();
        }
        else if (mState == EGameState::GameOver)
        {
            if (GetAsyncKeyState(KEY_ENTER))
            {
                mState = EGameState::BeforePlay;
            }
            else if (GetAsyncKeyState(KEY_BACKSPACE))
            {
                mState = EGameState::Menu;
            }
        }
    }
}

void Game::Update()
{
    while (mRunning)
    {
        gUpdateSemaphore.Take();
        if (mState == EGameState::Menu)
        {
            EMenuState menuState = mMenuManager->GetMenuState();
            setStateByMenuChoice(menuState);
        }
        else if (mState == EGameState::BeforePlay)
        {
            Reset();
            mGameManager->OnStart();
            mState = EGameState::Play;
        }
        else if (mState == EGameState::Play)
        {
            mGameManager->Update();
            if (!mGameManager->IsSnakeAlive())
            {
                mState = EGameState::GameOver;
            }
        }
        else if (mState == EGameState::Exit)
        {
            Stop();
        }
        gRenderSemaphore.Give();
    }
}

void Game::Render()
{
    while (mRunning)
    {
        gRenderSemaphore.Take();
        // clean screen
        system("cls");

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
            std::cout << "Game Over!" << std::endl;
            std::cout << "Score: " << mGameManager->GetScore() << std::endl;
            std::cout << "Rank: " << mGameManager->GetRank() << std::endl;
            std::cout << "Click ENTER to restart Game." << std::endl;
            std::cout << "Click BACKSPACE to return in Menu." << std::endl;
        }
        gUpdateSemaphore.Give();
    }
}

void Game::Reset()
{
    mGameManager->Reset(true);
    mMenuManager->Reset();
}

void Game::setStateByMenuChoice(EMenuState state)
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
