#include "GameEngine.h"

void GameEngine::Init()
{
    Events.Init();
}

void GameEngine::Shutdown()
{
    Events.Shutdown();
}