#include <ctime>

#include "Snake.h"
#include "GameParams.h"

Snake::Snake()
    : mBody()
    , mSpeed(0.0f)
    , mCurrentDir(ESnakeDirection::Up)
    , mAlive(true)
{
}

void Snake::Init(Position headPos, bool isDemoMode)
{
    mBody.push_back(headPos);

    if (isDemoMode)
    {
        mAi.Init();
    }
}

void Snake::Update()
{
    // if demo mode is active
    if (mAi.IsActive())
    {
        mCurrentDir = mAi.GenerateDirection(mBody[0], mCurrentDir);
    }
}

void Snake::Reset()
{
    mAlive = true;
    mBody.clear();
    mSpeed = 0.0f;
    mCurrentDir = ESnakeDirection::Up;
    mAi.Reset();
}

void Snake::ChangeDirection(ESnakeDirection newDir)
{
    // if snake is only the head
    if (mBody.size() == 1)
    {
        mCurrentDir = newDir;
        return;
    }

    switch (newDir)
    {
    case ESnakeDirection::Up:
        if (mCurrentDir != ESnakeDirection::Down)
            mCurrentDir = newDir;
        break;
    case ESnakeDirection::Down:
        if (mCurrentDir != ESnakeDirection::Up)
            mCurrentDir = newDir;
        break;
    case ESnakeDirection::Left:
        if (mCurrentDir != ESnakeDirection::Right)
            mCurrentDir = newDir;
        break;
    case ESnakeDirection::Right:
        if (mCurrentDir != ESnakeDirection::Left)
            mCurrentDir = newDir;
        break;
    }
}

AI::AI()
    : mActive(false)
    , mApplePos()
{
}

void AI::Init()
{
    mActive = true;
}

void AI::Reset()
{
    mActive = false;
}

void AI::SetApplePos(const Position& applePos)
{
    mApplePos = applePos;
}

ESnakeDirection AI::GenerateDirection(Position snakeHeadPos, ESnakeDirection currentDir)
{
    int snakeX = snakeHeadPos.GetX();
    int snakeY = snakeHeadPos.GetY();

    int appleX = mApplePos.GetX();
    int appleY = mApplePos.GetY();

    if (snakeX > appleX && currentDir != ESnakeDirection::Right)
    {
        return ESnakeDirection::Left;
    }
    if (snakeX < appleX && currentDir != ESnakeDirection::Left)
    {
        return ESnakeDirection::Right /*: ESnakeDirection::Down*/;
    }
    if (snakeY > appleY && currentDir != ESnakeDirection::Down)
    {
        return ESnakeDirection::Up;
    }
    if (snakeY < appleY && currentDir != ESnakeDirection::Up)
    {
        return ESnakeDirection::Down;
    }

    return currentDir;
}

void Snake::SetAIApplePos(const Position& applePos)
{
    mAi.SetApplePos(applePos);
}
