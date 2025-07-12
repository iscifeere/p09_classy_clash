#ifndef TEX_H
#define TEX_H

#include "raylib.h"

class Tex
{
public:
    Tex() = delete;

    static Texture2D texture_map;
    static Texture2D texture_heart;
    static Texture2D texture_coin;
    static Texture2D texture_gem_green;
    static Texture2D texture_sword;
    static Texture2D texture_prop_rock;
    static Texture2D texture_prop_log;
    static Texture2D texture_knight_idle;
    static Texture2D texture_knight_run;
    static Texture2D texture_goblin_idle;
    static Texture2D texture_goblin_run;
    static Texture2D texture_slime_idle;
    static Texture2D texture_slime_run;
    static Texture2D texture_red_idle;
    static Texture2D texture_red_run;

    // static void loadTextures();         // unused
    static void loadTexturesArr();
    // static void unloadTextures();       // unused
    static void unloadTexturesArr();

    constexpr static const float MAP_SCALE{8.f};      // scaling of the map
    static int winSize[2];                            // window dimensions

private:
    static const char* pathArray_textures[];
    static Texture2D* ptrArray_textures[];

};

#endif