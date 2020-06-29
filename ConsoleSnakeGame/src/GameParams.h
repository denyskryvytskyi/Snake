#pragma once

class GameParams
{
public:
    static GameParams& instance()
    {
        static GameParams* instance = new GameParams();
        return *instance;
    }

private:
    GameParams() = default;

public:
    const int mWidth = 60;
    const int mHeight = 28;

    const float mAppleGenerateTimeInterval = 50.0f;

    const float mSnakeMoveUpdateWaitTime = 1.0f; // in seconds
    const float mSnakeBonusSpeed = 0.2f; // in seconds

    const int mScoreRowIndex = 10;
    const int mRankRowIndex = 12;

    const float mAiChangeDirTimeInterval = 8.0f;
};
