#include "Mobs.h"
#include "EntityManager.h"

SlimeMob::SlimeMob(Vector2 pos, Character* playerPtr)
{
    worldPos = pos;
    setTarget(playerPtr);

    texture = &Tex::texture_slime_idle;
    idle = &Tex::texture_slime_idle;
    run = &Tex::texture_slime_run;

    width = texture->width / maxFrames;
    height = texture->height;

    speed = 4.f;
    health = 40.f;
    damagePerSec = 5.f;
    chaseRadius = 300.f;
}

// ===================

MadKnightMob::MadKnightMob(Vector2 pos, Character* playerPtr)
{
    worldPos = pos;
    setTarget(playerPtr);

    texture = &Tex::texture_knight_idle;
    idle = &Tex::texture_knight_idle;
    run = &Tex::texture_knight_run;

    width = texture->width / maxFrames;
    height = texture->height;

    speed = 9.f;
    health = 120.f;
    // damagePerSec = 10.f;
    // chaseRadius = 400.f;
}

// ===================

RedMob::RedMob(Vector2 pos, Character* playerPtr)
{
    worldPos = pos;
    setTarget(playerPtr);

    texture = &Tex::texture_red_idle;
    idle = &Tex::texture_red_idle;
    run = &Tex::texture_red_run;

    maxFrames = 4;
    width = texture->width / maxFrames;
    height = texture->height / 2;

    speed = 9.f;
    health = 120.f;
    // damagePerSec = 10.f;
    // chaseRadius = 400.f;
}