#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "EventManager.h"

class GameEngine
{
public:
    GameEngine() = default;

    void Init();
    void Shutdown();
    
    EventManager Events{};
    
private:

};

#endif