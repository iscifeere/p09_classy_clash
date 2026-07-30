#ifndef ATTACKS_H
#define ATTACKS_H


#include "raylib.h"

class SwordSlash
{
public:
    SwordSlash(){};

    void spawnReset(Vector2 position, float damage);
    void tick(float deltaTime);
    
    Rectangle getHitBox(){ return m_hitBox; }
    float getDamage(){ return m_damage; }
    bool getAlive(){ return m_alive; }
    
private:
    Rectangle m_hitBox{
        .width = 30.f,
        .height = 30.f
    };
    float m_damage{};
    float m_lifetime{};
    bool m_alive{false};
};

#endif