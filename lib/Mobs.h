#ifndef SLIMEMOB_H
#define SLIMEMOB_H

#include "Enemy.h"

class SlimeMob : public Enemy
{
public:
    SlimeMob(Vector2 pos, Character* playerPtr);
private:

};

// ==================

class MadKnightMob : public Enemy
{
public:
    MadKnightMob(Vector2 pos, Character* playerPtr);
private:

};

// ==================

class RedMob : public Enemy
{
public:
    RedMob(Vector2 pos, Character* playerPtr);
private:

};

#endif