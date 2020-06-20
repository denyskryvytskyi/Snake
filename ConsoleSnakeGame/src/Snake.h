#pragma once

#include <vector>
#include <memory>

struct Position
{
public:
    Position();
    Position(int x, int y);

    int GetX() const { return x; }
    int GetY() const { return y; }

    Position& operator+=(const Position& right)
    {
        x += right.x;
        y += right.y;
        return *this;
    }
    const Position operator+(const Position& right)
    {
        x += right.x;
        y += right.y;
        return *this;
    }

private:
    int x;
    int y;
};

class Snake
{
    //typedef std::shared_ptr<Position> PositionPtr;
    typedef std::vector<Position> PositionArray;

public:
    enum class EDirection
    {
        Up,
        Down,
        Left,
        Right,
    };

    enum class ECollisionType
    {
        None,
        Apple,
        Wall,
        Snake,
    };

public:
    Snake();
    ~Snake();
    //
    void Init(Position headPos);
    void Update();
    //
    Position GetHeadPos() const { return mBody[0]; }
    EDirection GetCurrentDir() const { return mCurrentDir; }
    PositionArray GetBody() const { return mBody; }
    int GetScore() const { return mScore; }
    //
    void ChangeDirection(EDirection newDir);
    //
    bool IsAlive() const { return mAlive; }

private:
    ECollisionType CheckCollision();
    void SwapPositions(Position* prev, Position* next);

private:
    PositionArray mBody; // indices of Map cells with state ECellState_Snake
    EDirection mCurrentDir; // direction of movement
    bool mAlive;

    float mSpeed;
    float mBonusSpeed;
    float mUpdateCheckpoint;

    int mScore;
};

