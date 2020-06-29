#pragma once

#include <map>

#include "Map.h"
#include "Snake.h"
#include "common_types.h"

class GameParams;

/*
* Timer class needed to implement mechanics:
* - snake speed
* - apple generation
*/
class SnakeTimer
{
public:
    SnakeTimer();

    void Init(float waitTime);
    bool IsOver() const { return mCurrentTime <= 0.0f; }
    void OnTick(float dt) { mCurrentTime -= dt; };
    void Restart(float bonusTime = 0.0f);

private:
    float mWaitTime; // wait time
    float mCurrentTime; // current time
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
    void Update(float dt);
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
    //float mAppleGenCheckpoint;
    float mBonusSpeed; // bonus snake speed for the eaten apple
    SnakeTimer mSpeedTimer;
    SnakeTimer mAppleGenerationTimer;
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