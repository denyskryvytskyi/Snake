#include <mutex>

#include "GameParams.h"

std::mutex mu;

GameParams* GameParams::instance = nullptr;

GameParams* GameParams::GetInstance()
{
    std::lock_guard<std::mutex> lock(mu);
    if (instance == nullptr)
    {
        instance = new GameParams();
    }
    return instance;
}