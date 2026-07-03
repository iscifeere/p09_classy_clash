#ifndef KNOCKBACK_H
#define KNOCKBACK_H

#include "raylib.h"
#include "Enemy.h"

class KnockbackForce
{
public:
    KnockbackForce(){};
    KnockbackForce(Vector2 p_direction, float p_magnitude, Enemy* p_target) :
    direction(p_direction), magnitude(p_magnitude), targetEnemy(p_target){};

    void reset(Vector2 p_direction, float p_magnitude, Enemy* p_target);
    void reset(Vector2 p_direction, float p_magnitude);

    void tick(float deltaTime);
    bool isActive(){ return active; }

    Enemy* getOwnerEnemy(){ return targetEnemy; }
    Vector2 getDirection(){ return direction; }
    float getMagnitude(){ return magnitude; }

private:
    bool active{false};
    Enemy* targetEnemy{nullptr};
    Vector2 direction{};
    float magnitude{};
    float timer{};

};

#endif