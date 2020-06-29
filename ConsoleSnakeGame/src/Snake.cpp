#include <ctime>

#include "Snake.h"
#include "GameParams.h"

Snake::Snake()
    : mBody()
    , mSpeed(0.0f)
    , mCurrentDir(EDirection::Up)
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
        mAi.Update();
        if (mAi.IsChangeDir())
        {
            EDirection newDir = GenerateRandomDirection();
            ChangeDirection(newDir);
            mAi.Reset(false);
        }
    }
}

void Snake::Reset()
{
    mAlive = true;
    mBody.clear();
    mSpeed = 0.0f;
    mCurrentDir = EDirection::Up;
    mAi.Reset();
}

void Snake::ChangeDirection(EDirection newDir)
{
    // if snake is only the head
    if (mBody.size() == 1)
    {
        mCurrentDir = newDir;
        return;
    }

    switch (newDir)
    {
    case EDirection::Up:
        if (mCurrentDir != EDirection::Down)
            mCurrentDir = newDir;
        break;
    case EDirection::Down:
        if (mCurrentDir != EDirection::Up)
            mCurrentDir = newDir;
        break;
    case EDirection::Left:
        if (mCurrentDir != EDirection::Right)
            mCurrentDir = newDir;
        break;
    case EDirection::Right:
        if (mCurrentDir != EDirection::Left)
            mCurrentDir = newDir;
        break;
    }
}

AI::AI()
    : mTimeToNextDirChange(0.0f)
    , mCurrentTimeCheckpoint(0.0f)
    , mNeedChangeDir(true)
    , mActive(false)
{
}

void AI::Init()
{
    mActive = true;
    mTimeToNextDirChange = GameParams::instance().mAiChangeDirTimeInterval;
    mCurrentTimeCheckpoint = mTimeToNextDirChange;
}

void AI::Update()
{
    if (mActive)
    {
        if (mCurrentTimeCheckpoint <= 0)
        {
            mNeedChangeDir = true;
            mCurrentTimeCheckpoint = mTimeToNextDirChange;
        }
        --mCurrentTimeCheckpoint;
    }
}

void AI::Reset(bool hardReset /*= true*/)
{
    mNeedChangeDir = false;

    if (hardReset)
    {
        mActive = false;
        mCurrentTimeCheckpoint = 0.0f;
    }
}

Snake::EDirection Snake::GenerateRandomDirection()
{
    srand(time(0));

    EDirection newRandDir = mCurrentDir;
    while (newRandDir == mCurrentDir)
    {
        newRandDir = static_cast<EDirection>(rand() % 4);
    }

    return newRandDir;
}
