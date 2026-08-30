#ifndef ENEMY_BEHAVIOUR_H
#define ENEMY_BEHAVIOUR_H

#include "Enemy.h"

class EnemyBehaviour
{
public:
    EnemyBehaviour() = delete;     // static class

    // IDLE LOGIC
    static void idleWandering(Enemy* thisEnemy, Character* player, const float& deltaTime);
    static void idleWanderingAlert(Enemy* thisEnemy, Character* player, const float& deltaTime);

    // TRANSITION LOGIC
    static void playerSpottedSequence(Enemy* thisEnemy, Character* player, const float& deltaTime);

    // CONFRONTATION LOGIC
    static void chaseTarget(Enemy* thisEnemy, Character* target, const float& deltaTime);
    static void fleeTarget(Enemy* thisEnemy, Character* target, const float& deltaTime);
    static void shootTarget(Enemy* thisEnemy, Character* target, const float& deltaTime);
};

#endif