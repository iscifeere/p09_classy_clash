#ifndef KNOCKBACK_H
#define KNOCKBACK_H

#include "raylib.h"
#include "BaseCharacter.h"

class KnockbackForce
{
public:
    KnockbackForce(){};
    KnockbackForce(Vector2 p_direction, float p_magnitude, BaseCharacter* p_target) :
    direction(p_direction), magnitude(p_magnitude), targetCharacter(p_target){};

    void reset(Vector2 p_direction, float p_magnitude, BaseCharacter* p_target);

    void tick(float deltaTime);
    bool isActive(){ return active; }

    BaseCharacter* getOwnerCharacter(){ return targetCharacter; }
    Vector2 getDirection(){ return direction; }
    float getMagnitude(){ return magnitude; }

private:
    bool active{false};
    BaseCharacter* targetCharacter{nullptr};
    Vector2 direction{};
    float magnitude{};
    float timer{};

};

#endif