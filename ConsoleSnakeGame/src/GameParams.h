#include <mutex>

std::mutex mu;

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
    const int mHeight = 20;

    const float mDeltaTime = 2.0f;

    const float mAppleGenerateTimeInterval = 50.0f;

    const float mSnakeMoveUpdateDt = 3.0f;
    const float mSnakeMoveUpdateGap = 0.4f;
    const float mSnakeBonusSpeed = 0.3f;

    const int mScoreRowIndex = 10;
    const int mRankRowIndex = 12;

private:
    static GameParams* instance;
};

GameParams* GameParams::instance = nullptr; 

GameParams* GameParams::GetInstance()
{
    std::lock_guard<std::mutex> lockMY(mu);
    if (instance == nullptr)
    {
        instance = new GameParams();
    }
    return instance;
}