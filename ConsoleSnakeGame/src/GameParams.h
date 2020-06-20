#pragma once
class GameParams
{
public:
    const int mWidth = 60;
    const int mHeight = 20;

    const float mDeltaTime = 2.0f;

    const float mAppleGenerateTimeInterval = 50.0f;

    const float mSnakeMoveUpdateDt = 3.0f;
    const float mSnakeMoveUpdateGap = 0.4f;
    const float mSnakeBonusSpeed = 0.3f;
};

extern GameParams gGameParams;