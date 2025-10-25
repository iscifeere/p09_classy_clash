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
#include "Game.h"

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

    g_MapPosCorrection = {
        static_cast<float>(Tex::winSize[0])*0.5f - (Tex::texture_knight_idle.width/6)*Tex::MAP_SCALE*0.5f,
        static_cast<float>(Tex::winSize[1])*0.5f - Tex::texture_knight_idle.height*Tex::MAP_SCALE*0.5f
    };

    EntityMng::initializeEntityManager();
    EntityMng::player.setWorldPos(Tex::halfWinSize);

    EntityMng::spawnProp(Vector2{1200.f, 800.f}, &ROCK_PROPDATA);
    EntityMng::spawnProp(Vector2{800.f, 1200.f}, &LOG_PROPDATA);
    EntityMng::spawnProp(Vector2{1800.f, 1200.f}, &SIGN_PROPDATA);

    // finish textures load | change to previous working directory
    ChangeDirectory(prev_dir);
    const std::string finishAssetsDirectory{ GetWorkingDirectory() };

    HideCursor();

    Game game{};

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        const float dT{ GetFrameTime() }; // delta time

        game.updateFrame(dT);

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