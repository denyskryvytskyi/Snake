#pragma once

#include <vector>
#include <memory>

#include "common_types.h"
#include "Position.h"

/*
* Component for the demo mode
*/
class AI
{
public:
    AI();
    //
    void Init();
    void Reset();
    //
    bool IsActive() const { return mActive; }
    //
    void SetApplePos(const Position& applePos);
    ESnakeDirection GenerateDirection(Position snakeHeadPos, ESnakeDirection currentDir);

private:
    bool mActive;
    Position mApplePos;
};

class Snake
{
public:
    Snake();
    //
    void Init(Position headPos, bool isDemoMode, float maxSpeed);
    void Update();
    void Reset();
    //
    Position GetHeadPos() const { return mBody[0]; }
    ESnakeDirection GetCurrentDir() const { return mCurrentDir; }
    PositionArrayPtr GetBody() { return &mBody; }
    void SetAlive(const bool alive) { mAlive = alive; }
    float GetSpeed() const { return mSpeed; }
    //
    void IncreaseSpeed(const float increaseSpeed) { if (mSpeed + increaseSpeed <= mMaxSpeed) mSpeed += increaseSpeed; }
    void ChangeDirection(ESnakeDirection newDir);
    //
    bool IsAlive() const { return mAlive; }
    //
    void SetAIApplePos(const Position& applePos);

private:
    PositionArray mBody;            // indices of Map cells with state ECellState_Snake
    float mSpeed;                   // current speed
    float mMaxSpeed;
    ESnakeDirection mCurrentDir;    // direction of movement
    bool mAlive;
    //
    AI mAi;                         // for the demo mode
};

