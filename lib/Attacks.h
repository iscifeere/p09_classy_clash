#ifndef ATTACKS_H
#define ATTACKS_H


#include "raylib.h"

class SwordSlash
{
public:
    SwordSlash(){};

    void spawnReset(Vector2 position, float damage);
    void tick(float deltaTime);
    void render(){};
    void showDebugData();
    
    Rectangle getHitBox(){ return m_HitBox; }
    float getDamage(){ return m_Damage; }
    bool getAlive(){ return m_Alive; }
    Vector2 getScreenPos();
    Vector2 getRenderPos();
    Vector2 getRenderPos(Vector2 screenPos);
    float getHeight(){ return m_HitBox.height; }
    
private:
    Vector2 m_Position{};
    Rectangle m_HitBox{
        .width = 90.f,
        .height = 90.f
    };
    float m_Damage{};
    float m_Lifetime{};
    bool m_Alive{false};
};

#endif