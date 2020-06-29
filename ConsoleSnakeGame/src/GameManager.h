#pragma once

#include <map>

#include "Map.h"
#include "Snake.h"
#include "common_types.h"

class GameParams;

/*
* Timer class needed to implement snake speed
*/
class Timer
{
public:
    Timer();

    void Init();
    bool IsOver() const { return mCheckpoint <= 0; }
    void OnTick() { mCheckpoint -= mCheckpointGap; };
    void Restart(int bonusTime = 0);

private:
    float mDt;              // update time
    float mCheckpoint;      // current update time
    float mCheckpointGap;   // gap until time is over
};

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
    //
    void Init();
    void OnStart(bool isDemoMode);
    void Update();
    void Render();
    void InputHandler();
    void Reset(bool hardReset = false);
    //
    bool IsSnakeAlive() const { return mSnake->IsAlive(); }
    //
    int GetScore() const { return mScore; }
    std::string GetRank() const { return mRank; }

private:
    ECollisionType checkCollision();
    void moveSnakeItem(const Position prevItemPos, const Position nextItemPos, bool isHead = false);
    void addSnakeItem(const Position newItemPos);
    //
    void swapPositions(Position* prev, Position* next);
    void generateApple();
    Position getRandStartSnakePos();
    int positionToMapIndex(const Position& pos) const;
    //
    void calcRank(); // calculate snake rank

private:
    Map* mMap;
    Snake* mSnake;
    //
    float mAppleGenCheckpoint;
    float mBonusSpeed; // bonus snake speed for the eaten apple
    Timer mTimer;
    //
    int mScore;
    std::string mRank;
    std::map<int, std::string> mRankNames;
    //
    int mScoreRowIndex; // row of map where will be drawn player score
    int mRankRowIndex;  // row of map where will be drawn player rank
    //
    bool mInputHanlderEnable;
    //
    bool mDemoMode;
};