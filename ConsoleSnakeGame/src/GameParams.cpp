#include "GameParams.h"

GameParams& GameParams::instance()
{
    static GameParams* instance = new GameParams();

    return *instance;
}