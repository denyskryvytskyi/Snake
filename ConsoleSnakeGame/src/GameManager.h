#pragma once

#include "Map.h"
#include "Snake.h"

class GameParams;

class GameManager
{
    enum class ECollisionType
    {
        None,
        Apple,
        Wall,
        Snake,
    };

public:
    GameManager();
    ~GameManager();

    void Init();
    void Update();
    void Render();
    //
    void InputHandler();
    //
    bool IsSnakeAlive() const { return mSnake->IsAlive(); }

private:
    ECollisionType checkCollision();
    void generateApple();
    //
    int positionToMapIndex(const Position& pos) const;
    void swapPositions(Position* prev, Position* next);
    void moveSnakeItem(const Position prevItemPos, const Position nextItemPos, bool isHead = false);
    void addSnakeItem(const Position newItemPos);

private:
    Map* mMap;
    Snake* mSnake;
    GameParams* mGameParams;
    //
    float mAppleGenCheckpoint;
    //
    float mSpeed;
    float mBonusSpeed;
    //
    float mUpdateDt;
    float mUpdateCheckpoint;
    float mUpdateCheckpointGap;
    //
    int mScore;
    //
    int mScoreRow;
    int mRankRow;
};