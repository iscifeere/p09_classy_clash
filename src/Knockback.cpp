#include "Knockback.h"
#include <iostream>


void KnockbackForce::reset(Vector2 p_direction, float p_magnitude, BaseCharacter* p_target)
{
    direction = p_direction;
    magnitude = p_magnitude;
    targetCharacter = p_target;
    timer = 0.1f;
    active = true;
}

void KnockbackForce::tick(float deltaTime)
{
    if(!active) return;

    // affect enemy
    targetCharacter->applyMovementForce(direction, magnitude);

    // decrease time until deactivating
    timer -= deltaTime;
    if(timer < 0.f)
    {
        active = false;
        timer = 0.f;
    }
}