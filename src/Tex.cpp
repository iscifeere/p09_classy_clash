#include "Tex.h"
#include <iostream>

int Tex::winSize[2]{};

Texture2D Tex::texture_map{};
Texture2D Tex::texture_heart{};
Texture2D Tex::texture_coin{};
Texture2D Tex::texture_gem_green{};
Texture2D Tex::texture_sword{};
Texture2D Tex::texture_prop_rock{};
Texture2D Tex::texture_prop_log{};
Texture2D Tex::texture_knight_idle{};
Texture2D Tex::texture_knight_run{};
Texture2D Tex::texture_goblin_idle{};
Texture2D Tex::texture_goblin_run{};
Texture2D Tex::texture_slime_idle{};
Texture2D Tex::texture_slime_run{};
Texture2D Tex::texture_red_idle{};
Texture2D Tex::texture_red_run{};

Texture2D* Tex::ptrArray_textures[]{        // array w/ every texture's ptr
    &texture_map,
    &texture_heart,
    &texture_coin,
    &texture_gem_green,
    &texture_sword,
    &texture_prop_rock,
    &texture_prop_log,
    &texture_knight_idle,
    &texture_knight_run,
    &texture_goblin_idle,
    &texture_goblin_run,
    &texture_slime_idle,
    &texture_slime_run,
    &texture_red_idle,
    &texture_red_run
};

const char* Tex::pathArray_textures[]{      // array w/ every texture's path to file
    "../assets/nature_tileset/mapaMundo.png",
    "../assets/characters/heart.png",
    "../assets/characters/MonedaD.png",
    "../assets/characters/gem_green.png",
    "../assets/characters/weapon_sword.png",
    "../assets/nature_tileset/Rock.png",
    "../assets/nature_tileset/Log.png",
    "../assets/characters/knight_idle_spritesheet.png",
    "../assets/characters/knight_run_spritesheet.png",
    "../assets/characters/goblin_idle_spritesheet.png",
    "../assets/characters/goblin_run_spritesheet.png",
    "../assets/characters/slime_idle_spritesheet.png",
    "../assets/characters/slime_run_spritesheet.png",
    "../assets/characters/Red_Idle.png",
    "../assets/characters/Red_Run.png"
};

// void Tex::loadTextures(){       // unused
//     std::cout << "=======[LOADING TEXTURES... (MANUALLY)]=======" << std::endl;
//     texture_map = LoadTexture("../assets/nature_tileset/mapaMundo.png");
//     texture_heart = LoadTexture("../assets/characters/heart.png");
//     texture_sword = LoadTexture("../assets/characters/weapon_sword.png");
//     texture_prop_rock = LoadTexture("../assets/nature_tileset/Rock.png");
//     texture_prop_log = LoadTexture("../assets/nature_tileset/Log.png");
//     texture_knight_idle = LoadTexture("../assets/characters/knight_idle_spritesheet.png");
//     texture_knight_run = LoadTexture("../assets/characters/knight_run_spritesheet.png");
//     texture_goblin_idle = LoadTexture("../assets/characters/goblin_idle_spritesheet.png");
//     texture_goblin_run = LoadTexture("../assets/characters/goblin_run_spritesheet.png");
//     texture_slime_idle = LoadTexture("../assets/characters/slime_idle_spritesheet.png");
//     texture_slime_run = LoadTexture("../assets/characters/slime_run_spritesheet.png");
//     std::cout << "=======[FINISHED TEXTURE LOADING!]=======" << std::endl;
// }

void Tex::loadTexturesArr(){        // Load Textures automatically from textures ptr array
    std::cout << "=======[LOADING TEXTURES... (FROM ARRAY)]=======" << std::endl;
    int i{0};
    for (auto texturePtr : ptrArray_textures){
        *texturePtr = LoadTexture(pathArray_textures[i]);
        i++;
    }
    i = 0;
    std::cout << "=======[FINISHED TEXTURE LOADING!]=======" << std::endl;
}

// void Tex::unloadTextures(){     // unused
//     std::cout << "=======[UNLOADING TEXTURES... (MANUALLY)]=======" << std::endl;
//     UnloadTexture(texture_map);
//     UnloadTexture(texture_heart);
//     UnloadTexture(texture_sword);
//     UnloadTexture(texture_prop_rock);
//     UnloadTexture(texture_prop_log);
//     UnloadTexture(texture_knight_idle);
//     UnloadTexture(texture_knight_run);
//     UnloadTexture(texture_goblin_idle);
//     UnloadTexture(texture_goblin_run);
//     UnloadTexture(texture_slime_idle);
//     UnloadTexture(texture_slime_run);
//     std::cout << "=======[FINISHED TEXTURE UNLOADING!]=======" << std::endl;
// }

void Tex::unloadTexturesArr(){
    std::cout << "=======[UNLOADING TEXTURES... (FROM ARRAY)]=======" << std::endl;
    // int i{0};
    for (auto texturePtr : ptrArray_textures){
        UnloadTexture(*texturePtr);
        // i++;
    }
    // i = 0;
    std::cout << "=======[FINISHED TEXTURE UNLOADING!]=======" << std::endl;
}