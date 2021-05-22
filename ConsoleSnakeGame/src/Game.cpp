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
    , mUpdateThread(&Game::Update, this)
    , mRenderThread(&Game::Render, this)
    , mIsForceCls(false)
{
    mMenuManager = new MenuManager();
    mGameManager = new GameManager();
}

Game::~Game()
{
    // wait when the threads execution has completed
    mUpdateThread.join();
    mRenderThread.join();

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
    // initialize all subsystems on game start
    Init();

    // now threads can be do the work
    mRunning = true;

    // input handling in the main thread
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
}

void Game::Update()
{
    mTimer.reset();
    float elapsedTime;
    while (mRunning)
    {
        // get deltatime as seconds
        elapsedTime = mTimer.elapsed();
        mTimer.reset();
        if (mState == EGameState::Menu)
        {
            // use semaphore to make synchronization with render thread
            gUpdateSemaphore.Take();
            EMenuState menuState = mMenuManager->GetMenuState();
            // notify render thread to continue execution
            gRenderSemaphore.Give();
            //
            // if menu state isn't "Navigation", then change game state
            setStateByMenuChoice(menuState);
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
            // use semaphore to make synchronization with render thread
            gUpdateSemaphore.Take();
            mGameManager->Update(elapsedTime);
            // notify render thread to continue execution
            gRenderSemaphore.Give();

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
}

void Game::Render()
{
    while (mRunning)
    {
        CleanConsole();

        if (mState == EGameState::Menu)
        {
            // use semaphore to make synchronization with update thread
            gRenderSemaphore.Take();
            mMenuManager->Render();
            // notify update thread to continue execution
            gUpdateSemaphore.Give();
        }
        else if (mState == EGameState::Play)
        {
            // use semaphore to make synchronization with update thread
            gRenderSemaphore.Take();
            mGameManager->Render();
            // notify update thread to continue execution
            gUpdateSemaphore.Give();
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
