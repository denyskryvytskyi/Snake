#include <windows.h>
#include <iostream>

#include "Game.h"
#include "GameManager.h"
#include "MenuManager.h"
#include "defines.h"

Game::Game()
    :mState(EGameState::Menu)
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

void Game::Init()
{
    // GameManager Init
    mGameManager->Init();
}

void Game::ProcessInput()
{
    // key handling
    if (mState == EGameState::Menu)
    {
        // call MenuManager InputHandler
        mMenuManager->InputHandler();
    }
    else if (mState == EGameState::Play)
    {
        // call GameManager InputHandler
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

void Game::Update()
{
    if (mState == EGameState::Menu)
    {
        EMenuState menuState = mMenuManager->GetMenuState();
        setStateByMenuChoice(menuState);
    }
    else if (mState == EGameState::Play)
    {
        // call GameManager Update
        mGameManager->Update();
        if (!mGameManager->IsSnakeAlive())
        {
            mState = EGameState::GameOver;
        }
    }
    else if (mState == EGameState::BeforePlay)
    {
        Reset();
        mGameManager->OnStart();
        mState = EGameState::Play;
    }
}

void Game::Render()
{
    // clean screen
    system("cls");

    if (mState == EGameState::Menu)
    {
        // call MenuManager Render
        mMenuManager->Render();
    }
    else if (mState == EGameState::Play)
    {
        // call GameManager Render
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
