#include "Knockback.h"
#include <iostream>


void KnockbackForce::reset(Vector2 p_direction, float p_magnitude, Enemy* p_target)
{
    direction = p_direction;
    magnitude = p_magnitude;
    targetEnemy = p_target;
    timer = 0.1f;
    active = true;
}

void KnockbackForce::reset(Vector2 p_direction, float p_magnitude)
{
    direction = p_direction;
    magnitude = p_magnitude;
    timer = 0.1f;
    active = true;
}

void KnockbackForce::tick(float deltaTime)
{
    if(!active) return;

    std::cout << "executing knockforce tick" << std::endl;

    // affect enemy
    targetEnemy->applyMovementForce(direction, magnitude);

    // decrease force until deactivating
    timer -= deltaTime;
    if(timer < 0.f)
    {
        active = false;
        timer = 0.f;
    }
}