#include "GameParams.h"

#include <mutex>

std::mutex mu;

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