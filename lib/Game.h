#ifndef GAME_H
#define GAME_H

#include "raylib.h"

class Game
{
public:
    Game(){};

    void updateFrame(float deltaTime);

    void changeScreen();

private:
    void titleScreen();
    void challengeScreen();
    void gameplayScreen(float deltaTime);
    void gameOverScreen();
    void victoryScreen();

    int gameState{};    // which screen to render
};

inline bool g_PauseGame{false};
inline bool g_ShowDebugData{false};
inline Vector2 g_MapPosCorrection{};

#endif // GAME_H