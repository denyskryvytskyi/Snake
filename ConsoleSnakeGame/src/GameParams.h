#pragma once

class GameParams
{
public:
    static GameParams& instance();

private:
    GameParams() = default;

public:
    const int mWidth = 60;
    const int mHeight = 28;

    const float mAppleGenerateTimeInterval = 50.0f;

    const float mSnakeMoveUpdateDt = 2.4f;
    const float mSnakeMoveUpdateGap = 0.4f;
    const float mSnakeBonusSpeed = 0.3f;

    const int mScoreRowIndex = 10;
    const int mRankRowIndex = 12;

    const float mAiChangeDirTimeInterval = 8.0f;
};
