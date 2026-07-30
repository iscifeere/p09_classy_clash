#include "Attacks.h"

void SwordSlash::spawnReset(Vector2 position, float damage)
{
    m_hitBox.x = position.x;
    m_hitBox.y = position.y;
    m_damage = damage;
    m_alive = true;
}

void SwordSlash::tick(float deltaTime)
{
    if(!m_alive) return;

    // TO DO
}