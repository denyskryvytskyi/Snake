#include <conio.h>
#include <ctime>
#include <windows.h>
#include <WinUser.h>

#include "Game.h"
#include "GameParams.h"

Map gMap;
GameParams gGameParams;

Game::Game()
    :mState(EGameState::Menu)
    , mSnake()
{
}

Game::~Game()
{
    delete mSnake;
}

void Game::Init()
{
    srand(time(0));
    int x = rand() % (gGameParams.mWidth - 2) + 1;
    int y = rand() % (gGameParams.mHeight - 2) + 1;
    Position startSnakeHeadPos(x, y);

    mSnake = new Snake();
    mSnake->Init(startSnakeHeadPos);
    gMap.Init(startSnakeHeadPos);
}

void Game::ProcessInput()
{
    if (_kbhit())
    {
        char pressedKey = _getch();

        // key handling
        if (mState == EGameState::Active)
        {
            Snake::EDirection newDir = mSnake->GetCurrentDir();
            switch (pressedKey)
            {
            case 'w':
                newDir = Snake::EDirection::Up;
                break;
            case 's':
                newDir = Snake::EDirection::Down;
                break;
            case 'a':
                newDir = Snake::EDirection::Left;
                break;
            case 'd':
                newDir = Snake::EDirection::Right;
                break;
            }
            mSnake->ChangeDirection(newDir);
        }
    }
}

void Game::Update()
{
    // Map reconfig
    mSnake->Update();
    if (!mSnake->IsAlive())
    {
        mState = EGameState::GameOver;
    }

    gMap.Update();
}

void Game::Render()
{
    // clean screen
    system("cls");

    // redraw cells
    gMap.Draw(mSnake->GetScore());
}
