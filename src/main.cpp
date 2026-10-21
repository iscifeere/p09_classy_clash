#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "Tex.h"
#include "AudioManager.h"
#include "EntityManager.h"
#include "GameEngine.h"
#include "Character.h"
#include "Prop.h"
#include "Enemy.h"
#include "Item.h"
#include "GenericEntity.h"
#include "Knockback.h"
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
    Tex::fullWinSize = Vector2{static_cast<float>(windowDimensions[0]), static_cast<float>(windowDimensions[1])};
    
    InitWindow(windowDimensions[0], windowDimensions[1], "ClassyClash");

    // LOAD ASSETS | change working directory
    ChangeDirectory(app_dir);
    const std::string loadingAssetsDirectory{ GetWorkingDirectory() };

    Tex::loadTexturesArr();     // load every texture into memory
    AudioManager::Init();
    GameEngine engine{};
    engine.Init();
    EntityMng::init(engine.Events);

    // SET EVENT SUBSCRIBERS
    {
        Subscriber audioSub{EventManager::CreateSubscriber(AudioManager::NAME, AudioManager::PlayEventSound)};
        engine.Events.SubscribeToEvent(EventID::HeartCollected, audioSub);
        engine.Events.SubscribeToEvent(EventID::CoinCollected, audioSub);
        engine.Events.SubscribeToEvent(EventID::GemCollected, audioSub);
        engine.Events.SubscribeToEvent(EventID::EnemyKilled, audioSub);
    }
    
    g_MapPosCorrection = {
        static_cast<float>(Tex::winSize[0])*0.5f - (Tex::texture_knight_idle.width/6)*Tex::MAP_SCALE*0.5f,
        static_cast<float>(Tex::winSize[1])*0.5f - Tex::texture_knight_idle.height*Tex::MAP_SCALE*0.5f
    };

    EntityMng::player.setWorldPos(Vector2Scale(Tex::halfWinSize, 2.f));

    EntityMng::spawnProp(Vector2{1584.f, 1184.f}, &ROCK_PROPDATA);
    EntityMng::spawnProp(Vector2{1184.f, 1584.f}, &LOG_PROPDATA);
    EntityMng::spawnProp(Vector2{2184.f, 1584.f}, &SIGN_PROPDATA);
    EntityMng::spawnProp(Vector2{4049.f, 1757.f}, &SIGN_PROPDATA);
    EntityMng::spawnProp(Vector2{4048.f, 3293.f}, &SIGN_PROPDATA);
    EntityMng::spawnProp(Vector2{3048.f, 2293.f}, &TREE_PROPDATA);

    // finish textures load | change to previous working directory
    ChangeDirectory(prev_dir);
    const std::string finishAssetsDirectory{ GetWorkingDirectory() };

    HideCursor();

    Game game{};

    SetTargetFPS(60);
    
    std::cout << "\nGAME LOOP START\n";
    std::cout << "=====================================================\n" << std::endl;

    while (!WindowShouldClose())
    {
        if(IsKeyPressed(KEY_M))
        {
            GetMasterVolume() > 0.f ? SetMasterVolume(0.f) : SetMasterVolume(1.f);
        }
        
        BeginDrawing();
        ClearBackground(RAYWHITE);

        const float dT{ GetFrameTime() }; // delta time

        game.updateFrame(dT);

        EndDrawing();
    }

    std::cout << "\n=====================================================\n";
    std::cout << "GAME LOOP END\n" << std::endl;

    // DELETE EVENT SUBSCRIBERS
    engine.Events.DesubscribeFromEvent(EventID::HeartCollected, AudioManager::NAME);
    engine.Events.DesubscribeFromEvent(EventID::CoinCollected, AudioManager::NAME);
    engine.Events.DesubscribeFromEvent(EventID::GemCollected, AudioManager::NAME);
    engine.Events.DesubscribeFromEvent(EventID::EnemyKilled, AudioManager::NAME);

    // UNLOAD ASSETS
    Tex::unloadTexturesArr();
    AudioManager::Shutdown();
    engine.Shutdown();

    CloseWindow();

    // PRINT WORKING DIRECTORIES
    // std::cout << "Original working dir : " << prev_dir << std::endl;
    // std::cout << "Original application dir : "<<  app_dir << std::endl;
    // std::cout << "Working dir while loading assets : " << loadingAssetsDirectory << std::endl;
    // std::cout << "Working dir after loading assets : " << finishAssetsDirectory << std::endl;

    return 0;
}