#pragma once
#include <map>

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
    //
    void Init();
    void OnStart(bool isDemoMode);
    void Update();
    void Render();
    void Reset(bool hardReset = false);
    //
    void InputHandler();
    //
    bool IsSnakeAlive() const { return mSnake->IsAlive(); }
    //
    int GetScore() const { return mScore; }
    std::string GetRank() const { return mRank; }

private:
    ECollisionType checkCollision();
    void generateApple();
    //
    int positionToMapIndex(const Position& pos) const;
    void swapPositions(Position* prev, Position* next);
    void moveSnakeItem(const Position prevItemPos, const Position nextItemPos, bool isHead = false);
    void addSnakeItem(const Position newItemPos);
    Position getRandStartSnakePos();
    void CalcRank(); // calculate snake rank

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
    std::string mRank;
    std::map<int, std::string> mRankNames;
    //
    int mScoreRowIndex;
    int mRankRowIndex;
    //
    bool mInputHanlderEnable = true;
};