#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "Tex.h"
#include "Character.h"
#include "Prop.h"
#include "Enemy.h"
#include "Mobs.h"
#include "Item.h"
#include "GenericEntity.h"
#include "EntityManager.h"
#include <string>
#include <cmath>

const char *prev_dir{ GetWorkingDirectory() };
const char *app_dir{ GetApplicationDirectory() };

int main(void) {

    int windowDimensions[2]{ 768 , 768 }; // width , height
    Tex::winSize[0] = windowDimensions[0];
    Tex::winSize[1] = windowDimensions[1];
    Tex::halfWinSize = Vector2{static_cast<float>(Tex::winSize[0])*0.5f,static_cast<float>(Tex::winSize[1])*0.5f};
    
    InitWindow(windowDimensions[0], windowDimensions[1], "ClassyClash");

    // LOAD TEXTURES | change working directory
    ChangeDirectory(app_dir);
    const std::string loadingAssetsDirectory{ GetWorkingDirectory() };

    Tex::loadTexturesArr();     // load every texture into memory

    Vector2 mapPos{0,0};
    Vector2 mapPosCorrection{
        static_cast<float>(Tex::winSize[0])*0.5f - (Tex::texture_knight_idle.width/6)*Tex::MAP_SCALE*0.5f,
        static_cast<float>(Tex::winSize[1])*0.5f - Tex::texture_knight_idle.height*Tex::MAP_SCALE*0.5f
    };

    Character knight{Tex::halfWinSize};

    Vector2 playerWorPos{knight.getWorldPos()};

    EntityMng::spawnProp(Vector2{1200.f, 800.f}, &ROCK_PROPDATA, &knight);
    EntityMng::spawnProp(Vector2{800.f, 1200.f}, &LOG_PROPDATA, &knight);
    EntityMng::spawnProp(Vector2{1800.f, 1200.f}, &SIGN_PROPDATA, &knight);

    // finish textures load | change to previous working directory
    ChangeDirectory(prev_dir);
    const std::string finishAssetsDirectory{ GetWorkingDirectory() };

    // cursor
    Vector2 cursorPosition{};
    Color cursorColor{BLUE};

    bool showDebugData{false};

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        const float dT{ GetFrameTime() }; // delta time

        cursorPosition = GetMousePosition();
        HideCursor();

        // GAME LOGIC BEGINS ==========================

        // update playerWorldPosition
        Vector2 playerWorPos = knight.getWorldPos();

        // create items
        if( IsKeyPressed(KEY_E) ) EntityMng::spawnItem(Vector2Add(Vector2Subtract(playerWorPos, Tex::halfWinSize), cursorPosition), &knight, &COIN_ITEMDATA);

        // create enemies
        if( IsKeyPressed(KEY_R) ) EntityMng::spawnEnemy(Vector2Add(Vector2Subtract(playerWorPos, Tex::halfWinSize), cursorPosition), &knight, &MADKNIGHT_ENEMYDATA);
        if( IsKeyPressed(KEY_T) ) EntityMng::spawnEnemy(Vector2Add(Vector2Subtract(playerWorPos, Tex::halfWinSize), cursorPosition), &knight, &SLIME_ENEMYDATA);
        if( IsKeyPressed(KEY_Y) ) EntityMng::spawnEnemy(Vector2Add(Vector2Subtract(playerWorPos, Tex::halfWinSize), cursorPosition), &knight, &RED_ENEMYDATA);

        // delete hearts
        if( IsKeyPressed(KEY_Q) ) EntityMng::killItem();

        // delete enemies
        if( IsKeyPressed(KEY_F) ) EntityMng::killEnemy();

        // entities tick ========================
        // knight.tick(dT);
        // EntityMng::tickItems(dT);
        // EntityMng::tickEnemies(dT);
        // EntityMng::tickProyectiles(dT);
        EntityMng::tickEntities(dT, &knight);

        // cursor affects player
        if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
            cursorColor = YELLOW;
            if(CheckCollisionRecs(Rectangle{cursorPosition.x - 5, cursorPosition.y - 5, 5, 5}, knight.getCollisionRec())){
                knight.addHealth(30.f*dT);
            }
        } else if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            cursorColor = YELLOW;
            if(CheckCollisionRecs(Rectangle{cursorPosition.x - 5, cursorPosition.y - 5, 5, 5}, knight.getCollisionRec())){
                knight.takeDamage(30.f*dT);
            }
        } else cursorColor = DARKBLUE;

        EntityMng::checkPropCollisions(&knight);

        // update map position
        mapPos = Vector2Scale(Vector2Subtract(knight.getWorldPos(), mapPosCorrection), -1.f);
        // draw the map
        DrawTextureEx(Tex::texture_map, mapPos, 0.0, Tex::MAP_SCALE, WHITE);

        // reset game with key
        if(IsKeyPressed(KEY_J)){
            knight.resetState();
            EntityMng::clearEntityPools();
            EndDrawing();
            continue;
        }

        // draw game over text if dead
        if(!knight.getAlive()){
            DrawText("Game Over! (press J to reset)", 70.f, static_cast<float>((windowDimensions[1]/2)-20), 40, RED);
            if(IsKeyPressed(KEY_H)) knight.resurrect();
            EndDrawing();
            continue;
        }

        // ENTITIES RENDER =====================

        EntityMng::renderEntities(&knight);
        
        // draw player stats
        knight.showStats();

        // draw debug data
        if(IsKeyPressed(KEY_Z)) showDebugData = !showDebugData;
        if(showDebugData){
            knight.showDebugData();
            EntityMng::showEnemiesDebugData();
            EntityMng::showItemsDebugData();
            EntityMng::showProyectilesDebugData();
            EntityMng::showPropsDebugData();

            // draw map borders
            {
                Vector2 windowOriginWorPos = Vector2Subtract(knight.getWorldPos(), Tex::halfWinSize);
                Vector2 knightHalfSize{knight.getWidth()*0.5f, knight.getHeight()*0.5f};
                Vector2 mapScaledSize{ Tex::texture_map.width * Tex::MAP_SCALE, Tex::texture_map.height * Tex::MAP_SCALE };

                // bound points absolute position (world pos)
                Vector2 topLeftBoundWorPos = Vector2Subtract(Tex::halfWinSize, knightHalfSize);
                Vector2 topRightBoundWorPos{
                    (mapScaledSize.x - Tex::halfWinSize.x) - knightHalfSize.x,
                    Tex::halfWinSize.y - knightHalfSize.y
                };
                Vector2 bottomLeftBoundWorPos{
                    Tex::halfWinSize.x - knightHalfSize.x,
                    (mapScaledSize.y - Tex::halfWinSize.y) - knightHalfSize.y
                };
                Vector2 bottomRightBoundWorPos{
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

        // draw cursor
        DrawCircleV(cursorPosition, 10, cursorColor);

        // GAME LOGIC ENDS =================================

        EndDrawing();
    }

    // UNLOAD TEXTURES
    Tex::unloadTexturesArr();

    CloseWindow();

    // PRINT WORKING DIRECTORIES
    // std::cout << "Original working dir : " << prev_dir << std::endl;
    // std::cout << "Original application dir : "<<  app_dir << std::endl;
    // std::cout << "Working dir while loading assets : " << loadingAssetsDirectory << std::endl;
    // std::cout << "Working dir after loading assets : " << finishAssetsDirectory << std::endl;

    return 0;
}