#include "Attacks.h"
#include "raymath.h"
#include "EntityManager.h"

void SwordSlash::spawnReset(Vector2 position, float damage)
{
    m_Position = position;
    m_Damage = damage;
    m_Lifetime = 1.f;
    m_Alive = true;
}

void SwordSlash::tick(float deltaTime)
{
    if(!m_Alive) return;
    
    m_Lifetime > 0.f ? m_Lifetime -= deltaTime : m_Alive = false;
}

Vector2 SwordSlash::getScreenPos()
{
    return Vector2Subtract(m_Position, EntityMng::player.getWindowOriginWorPos());
}

Vector2 SwordSlash::getRenderPos()
{
    Vector2 screenPos = getScreenPos();
    return Vector2{
        screenPos.x - (m_HitBox.width * 0.5f),
        screenPos.y - (m_HitBox.height * 0.5f)
    };
}

Vector2 SwordSlash::getRenderPos(Vector2 screenPos)
{
    return Vector2{
        screenPos.x - (m_HitBox.width * 0.5f),
        screenPos.y - (m_HitBox.height * 0.5f)
    };
}

void SwordSlash::showDebugData(){
    Vector2 screenPos = getScreenPos();
    Vector2 renderPos = getRenderPos(screenPos);

    DrawRectangle(renderPos.x, renderPos.y, m_HitBox.width, m_HitBox.height, Fade(RED, 0.3f));
    DrawRectangleLines(renderPos.x, renderPos.y, m_HitBox.width, m_HitBox.height, YELLOW);
    DrawCircleV(screenPos, 5.f, BLUE);
}