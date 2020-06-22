#pragma once

#include <vector>
#include <memory>

#include "Position.h"

class AI
{
public:
    AI();
    //
    void Init();
    void Update();
    void Reset();
    //
    bool IsActive() const { return mActive; }
    bool IsChangeDir() const { return mNeedChangeDir; }

private:
    float mTimeToNextGen;
    float mCurrentTimeCheckpoint;
    bool mNeedChangeDir;
    bool mActive;
};

class Snake
{
public:
    enum class EDirection
    {
        Up,
        Down,
        Left,
        Right,
    };

public:
    Snake();
    //
    void Init(Position headPos, bool isDemoMode);
    void Update();
    void Reset();
    //
    Position GetHeadPos() const { return mBody[0]; }
    EDirection GetCurrentDir() const { return mCurrentDir; }
    PositionArrayPtr GetBody() { return &mBody; }
    void SetAlive(const bool alive) { mAlive = alive; }
    //
    void ChangeDirection(EDirection newDir);
    //
    bool IsAlive() const { return mAlive; }
    //
    EDirection GenerateRandomDirection();

private:
    PositionArray mBody; // indices of Map cells with state ECellState_Snake
    EDirection mCurrentDir; // direction of movement
    bool mAlive;
    //
    AI mAi; // for the demo mode
};

