#pragma once

class GameParams
{
public:
    static GameParams* GetInstance();

    GameParams(const GameParams&) = delete;
    GameParams& operator=(const GameParams&) = delete;

private:
    GameParams() = default;

public:
    const int mWidth = 60;
    const int mHeight = 28;

    const float mDeltaTime = 2.0f;

    const float mAppleGenerateTimeInterval = 50.0f;

    const float mSnakeMoveUpdateDt = 2.8f;
    const float mSnakeMoveUpdateGap = 0.4f;
    const float mSnakeBonusSpeed = 0.3f;

    const int mScoreRowIndex = 10;
    const int mRankRowIndex = 12;

    const float mAiChangeDirTimeInterval = 40.0f;

private:
    static GameParams* instance;
};
