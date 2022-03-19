#pragma once

class GameParams
{
public:
    static GameParams* instance()
    {
        static GameParams* instance = new GameParams();
        return instance;
    }

private:
    GameParams() = default;

public:
    const int mWidth = 60;
    const int mHeight = 28;

    const float mAppleGenerationTimeInterval = 3.0f; // as seconds

    const float mSnakeMoveUpdateWaitTime = 1.0f; // as seconds
    const float mSnakeBonusSpeed = 0.15f; // as seconds
    const float mSnakeMaxSpeed = 1.0f; // as seconds

    const int mScoreRowIndex = 10;
    const int mRankRowIndex = 12;

    const float mAiChangeDirTimeInterval = 8.0f;
};
