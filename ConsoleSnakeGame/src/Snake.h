#pragma once

#include <vector>
#include <memory>

#include "Position.h"

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
    void Init(Position headPos);
    //
    Position GetHeadPos() const { return mBody[0]; }
    EDirection GetCurrentDir() const { return mCurrentDir; }
    PositionArrayPtr GetBody() { return &mBody; }
    void SetAlive(const bool alive) { mAlive = alive; }
    //
    void ChangeDirection(EDirection newDir);
    //
    bool IsAlive() const { return mAlive; }

private:
    PositionArray mBody; // indices of Map cells with state ECellState_Snake
    EDirection mCurrentDir; // direction of movement
    bool mAlive;
};

