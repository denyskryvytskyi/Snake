#include <Windows.h>

#include "Game.h"

Game::Game()
    :mState(EGameState::Active)
{
    mGameManager = new GameManager;
}

Game::~Game()
{
    if (mGameManager != nullptr)
    {
        delete mGameManager;
    }
}

void Game::Init()
{
    // MenuManager Init
    // GameManager Init
    mGameManager->Init();
}

void Game::ProcessInput()
{
    // key handling
    if (mState == EGameState::Menu)
    {
        // call MenuManager InputHandler
    }
    else if (mState == EGameState::Active)
    {
        // call GameManager InputHandler
        mGameManager->InputHandler();
    }
}

void Game::Update()
{
    if (mState == EGameState::Menu)
    {
        // call MenuManager Update
    }
    else if (mState == EGameState::Active)
    {
        // call GameManager Update
        mGameManager->Update();
        if (!mGameManager->IsSnakeAlive())
        {
            mState = EGameState::GameOver;
        }
    }
}

void Game::Render()
{
    // clean screen
    system("cls");

    if (mState == EGameState::Menu)
    {
        // call MenuManager Render
    }
    else if (mState == EGameState::Active)
    {
        // call GameManager Render
        mGameManager->Render();
    }
}
