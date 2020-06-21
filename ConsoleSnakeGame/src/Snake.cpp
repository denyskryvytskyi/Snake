#include "Snake.h"

Snake::Snake()
    : mBody()
    , mCurrentDir(EDirection::Up)
    , mAlive(true)
{
}

void Snake::Init(Position headPos)
{
    mBody.push_back(headPos);
}

void Snake::Reset()
{
    mAlive = true;
    mBody.clear();
    mCurrentDir = EDirection::Up;
}

void Snake::ChangeDirection(EDirection newDir)
{
    // if snake is only head
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