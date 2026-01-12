#include "Game.h"
#include "Tex.h"
#include "EntityManager.h"

#define TITLE_SCREEN 0
#define GAME_PLAY 1
#define GAME_OVER 2
#define VICTORY_SCREEN 3
#define CHALLENGE_SCREEN 4

void Game::updateFrame(float deltaTime)
{
    // if(IsKeyPressed(KEY_SPACE)) changeScreen();

    switch (gameState)
    {
    case TITLE_SCREEN:
        titleScreen();
        break;

    case CHALLENGE_SCREEN:
        challengeScreen();
        break;

    case GAME_PLAY:
        gameplayScreen(deltaTime);
        break;

    case GAME_OVER:
        gameOverScreen();
        break;

    case VICTORY_SCREEN:
        victoryScreen();
        break;
    
    default:
        break;
    }
}

void Game::titleScreen()
{
    ClearBackground(SKYBLUE);

    if(IsKeyPressed(KEY_ENTER)) gameState = CHALLENGE_SCREEN;

    DrawText("Classy Clash", static_cast<float>(Tex::winSize[0])*0.33f, static_cast<float>(Tex::winSize[1])*0.33f, 50, WHITE);
    DrawText("(press Enter)", Tex::halfWinSize.x-70.f, Tex::halfWinSize.y+110.f, 30, YELLOW);
}

void Game::challengeScreen()
{
    ClearBackground(BLACK);

    DrawText("Challenge: Kill 5 enemies", static_cast<float>(Tex::winSize[0])*0.15f, static_cast<float>(Tex::winSize[1])*0.45f, 50, RED);
    DrawText("(press Enter)", static_cast<float>(Tex::winSize[0])*0.33f, static_cast<float>(Tex::winSize[1])*0.7f, 30, YELLOW);
    
    if(IsKeyPressed(KEY_ENTER))
    {
        gameState = GAME_PLAY;
        EntityMng::spawnRandomEnemies();
    }
}

void Game::gameplayScreen(float deltaTime)
{
    ClearBackground(PURPLE);

    const int FPS{GetFPS()};

    // cursor
    Vector2 cursorPosition{GetMousePosition()};
    Color cursorColor{DARKBLUE};

    // GAME LOGIC BEGINS ==========================

    // create items
    if(IsKeyPressed(KEY_E)) EntityMng::spawnItem(Vector2Add(EntityMng::player.getWindowOriginWorPos(), cursorPosition), &COIN_ITEMDATA);

    // create enemies
    if(IsKeyPressed(KEY_R)) EntityMng::spawnEnemy(Vector2Add(EntityMng::player.getWindowOriginWorPos(), cursorPosition), &MADKNIGHT_ENEMYDATA);
    if(IsKeyPressed(KEY_T)) EntityMng::spawnEnemy(Vector2Add(EntityMng::player.getWindowOriginWorPos(), cursorPosition), &SLIME_ENEMYDATA);
    if(IsKeyPressed(KEY_Y)) EntityMng::spawnEnemy(Vector2Add(EntityMng::player.getWindowOriginWorPos(), cursorPosition), &RED_ENEMYDATA);

    // delete items
    if(IsKeyPressed(KEY_Q)) EntityMng::killItem();

    // delete enemies
    if(IsKeyPressed(KEY_F)) EntityMng::killEnemy();

    if(IsKeyPressed(KEY_P)) g_PauseGame = !g_PauseGame;

    // ENTITIES TICK ========================
    if(!g_PauseGame)
    {
        EntityMng::tickEntities(deltaTime);
        EntityMng::checkEntityCollisions();
    }

    if(IsKeyPressed(KEY_N)) EntityMng::logEntityArrayStatus();

    // cursor affects player
    if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        cursorColor = YELLOW;
        if(CheckCollisionRecs(Rectangle{cursorPosition.x - 5.f, cursorPosition.y - 5.f, 10, 10}, EntityMng::player.getCollisionRec()))
        {
            EntityMng::player.addHealth(30.f*deltaTime);
        }
    }
    else if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        cursorColor = YELLOW;
        if(CheckCollisionRecs(Rectangle{cursorPosition.x - 5.f, cursorPosition.y - 5.f, 10, 10}, EntityMng::player.getCollisionRec()))
        {
            EntityMng::player.takeDamage(30.f*deltaTime);
        }
    }
    else cursorColor = DARKBLUE;

    // update map position
    Vector2 mapPos = Vector2Scale(Vector2Subtract(EntityMng::player.getWorldPos(), g_MapPosCorrection), -1.f);
    // draw the map
    DrawTextureEx(Tex::texture_map, mapPos, 0.0, Tex::MAP_SCALE, WHITE);

    // reset game with key
    if(IsKeyPressed(KEY_J))
    {
        EntityMng::player.resetState();
        EntityMng::clearEntityPools();
        EntityMng::spawnRandomEnemies();
        g_PauseGame = false;
        return;
    }

    // game over if dead
    if(!EntityMng::player.getAlive())
    {
        gameState = GAME_OVER;  // go to game over screen
        gameOverScreen();
        return;
    }
    else if(EntityMng::player.getWinCondition())
    {
        gameState = VICTORY_SCREEN;     // if win condition is true, go to victory screen
        victoryScreen();
        return;
    }

    // ENTITIES RENDER =====================
    EntityMng::renderEntities();
    
    // draw player stats
    EntityMng::player.showStats();
    EntityMng::showPlayerScore();

    // draw debug data
    if(IsKeyPressed(KEY_Z)) g_ShowDebugData = !g_ShowDebugData;
    if(g_ShowDebugData)
    {
        EntityMng::showEntitiesDebugData();

        DrawText(TextFormat("frameTime(ms): %01.02f",deltaTime*1000), 200.f, 125.f, 30, WHITE);
        DrawText(TextFormat("FPS: %02i",FPS), 200.f, 155.f, 30, WHITE);

        // draw map borders
        {
            Vector2 windowOriginWorPos = EntityMng::player.getWindowOriginWorPos();
            Vector2 knightHalfSize{EntityMng::player.getWidth()*0.5f, EntityMng::player.getHeight()*0.5f};
            Vector2 mapScaledSize{ Tex::texture_map.width * Tex::MAP_SCALE, Tex::texture_map.height * Tex::MAP_SCALE };

            // bound points absolute position (world pos)
            Vector2 topLeftBoundWorPos = Vector2Subtract(Tex::halfWinSize, knightHalfSize);
            Vector2 topRightBoundWorPos
            {
                (mapScaledSize.x - Tex::halfWinSize.x) - knightHalfSize.x,
                Tex::halfWinSize.y - knightHalfSize.y
            };
            Vector2 bottomLeftBoundWorPos
            {
                Tex::halfWinSize.x - knightHalfSize.x,
                (mapScaledSize.y - Tex::halfWinSize.y) - knightHalfSize.y
            };
            Vector2 bottomRightBoundWorPos
            {
                (mapScaledSize.x - Tex::halfWinSize.x) - knightHalfSize.x,
                (mapScaledSize.y - Tex::halfWinSize.y) - knightHalfSize.y
            };
            
            // bound points relative position (screen pos)
            Vector2 topLeftBoundScreenPos = Vector2Subtract(topLeftBoundWorPos, windowOriginWorPos);
            Vector2 topRightBoundScreenPos = Vector2Subtract(topRightBoundWorPos, windowOriginWorPos);
            Vector2 bottomLeftBoundScreenPos = Vector2Subtract(bottomLeftBoundWorPos, windowOriginWorPos);
            Vector2 bottomRightBoundScreenPos = Vector2Subtract(bottomRightBoundWorPos, windowOriginWorPos);

            // top horizontal
            DrawLine(topLeftBoundScreenPos.x, topLeftBoundScreenPos.y, topRightBoundScreenPos.x, topRightBoundScreenPos.y, YELLOW);
            // bottom horizontal
            DrawLine(bottomLeftBoundScreenPos.x, bottomLeftBoundScreenPos.y, bottomRightBoundScreenPos.x, bottomRightBoundScreenPos.y, YELLOW);
            // left vertical
            DrawLine(topLeftBoundScreenPos.x, topLeftBoundScreenPos.y, bottomLeftBoundScreenPos.x, bottomLeftBoundScreenPos.y, YELLOW);
            // right vertical
            DrawLine(topRightBoundScreenPos.x, topRightBoundScreenPos.y, bottomRightBoundScreenPos.x, bottomRightBoundScreenPos.y, YELLOW);
        }
    }

    if(g_PauseGame)
    {
        DrawRectangle(0, 0, Tex::winSize[0], Tex::winSize[1], Color{0,0,0,125});
        DrawText("game paused", Tex::winSize[0]-220, 60, 30, WHITE);
    }

    // draw cursor
    DrawCircleV(cursorPosition, 10, cursorColor);

    // GAME LOGIC ENDS =================================

}

void Game::gameOverScreen()
{
    ClearBackground(BLACK);

    if(IsKeyPressed(KEY_H))     // resurrect player
    {
        EntityMng::player.resurrect();
        gameState = GAME_PLAY;
    }
    else if(IsKeyPressed(KEY_J))    // reset game
    {
        EntityMng::player.resetState();
        EntityMng::clearEntityPools();
        gameState = GAME_PLAY;
    }
    else if(IsKeyPressed(KEY_ENTER))    // reset and return to title screen
    {
        EntityMng::player.resetState();
        EntityMng::clearEntityPools();
        gameState = TITLE_SCREEN;
    }

    DrawText("Game Over!", Tex::halfWinSize.x-200.f, Tex::halfWinSize.y-20.f, 60, WHITE);
    DrawText("(press J to reset)", 70.f, Tex::halfWinSize.y+110.f, 20, YELLOW);
    DrawText("(press Enter to return to title screen)", 70.f, Tex::halfWinSize.y+140.f, 20, YELLOW);
}

void Game::victoryScreen()
{
    ClearBackground(BLUE);

    if(IsKeyPressed(KEY_J))    // reset game
    {
        EntityMng::player.resetState();
        EntityMng::clearEntityPools();
        gameState = GAME_PLAY;
    }
    else if(IsKeyPressed(KEY_ENTER))    // reset and return to title screen
    {
        EntityMng::player.resetState();
        EntityMng::clearEntityPools();
        gameState = TITLE_SCREEN;
    }

    DrawText("You won!", Tex::halfWinSize.x-200.f, Tex::halfWinSize.y-20.f, 60, YELLOW);
    DrawText("(press J to reset)", 70.f, Tex::halfWinSize.y+110.f, 20, YELLOW);
    DrawText("(press Enter to return to title screen)", 70.f, Tex::halfWinSize.y+140.f, 20, YELLOW);
}

void Game::changeScreen()
{
    gameState++;
    if(gameState > 4) gameState = 0;
}