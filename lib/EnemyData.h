#ifndef ENEMYDATA_H
#define ENEMYDATA_H

#include "Enemy.h"
#include "EntityManager.h"

struct enemyData{
    Texture2D* texture_idle;
    Texture2D* texture_run;
    int maxFrames;
    int frameRows;
    float speed;
    float health;
    float damage;
    float chase_radius;
    const itemData* item_drop;
    void(*behave)(Enemy* enemy, Character* player, const float& deltaTime);
    bool isNeutral{false};
};

void shootTargetA(Enemy* enemy, Character* target, const float& deltaTime)
{
    Vector2& velocity = enemy->getVelocity();
    float& attCooldown = enemy->attackTimer;
    GenEntity*& proyectile = enemy->proyectile;

    // get target direction
    velocity = Vector2Subtract(target->getScreenPos(), enemy->getScreenPos());

    if(Vector2Length(velocity) > 400.f){
        EntityMng::tickProyectile(proyectile, deltaTime);
        return;
    }

    // if in radius -> stop and shoot
    if(Vector2Length(velocity) >= 330.f)
    {
        if(attCooldown == 0.f){     // if cooldown is off -> shoot proyectile
            EntityMng::spawnProyectile(proyectile, enemy->getWorldPos(), velocity, target);
            attCooldown += deltaTime;
        } else if (attCooldown >= 0.8f) attCooldown = 0.f;
        else attCooldown += deltaTime;

        velocity = {};
        enemy->setDrawColor(BLUE);
    }
    else velocity = Vector2Scale(velocity, -1.f);        // if too close -> get away

    EntityMng::tickProyectile(proyectile, deltaTime);
}
void shootTargetB(Enemy* enemy, Character* target, const float& deltaTime)
{
    Vector2& velocity = enemy->getVelocity();
    float& attCooldown = enemy->attackTimer;
    GenEntity*& proyectile = enemy->proyectile;
    float& fleeTimer = enemy->fleeTimer;
    float& chaseTimer = enemy->getRadiusEtc(2);

    // get target direction
    velocity = Vector2Subtract(target->getScreenPos(), enemy->getScreenPos());
    float distance = Vector2Length(velocity);       // distance from enemy to target

    if(enemy->flee){
        velocity = Vector2Scale(velocity, -1.f);
        fleeTimer += deltaTime;
        if(fleeTimer >= 0.7f && distance > 380.f) {fleeTimer = 0.f; enemy->flee = false;}
    }
        
    if(distance > 430.f){
        enemy->chase = true;
        EntityMng::tickProyectile(proyectile, deltaTime);
        chaseTimer += deltaTime;
        if(chaseTimer < 0.2f){
            velocity = {};
        }
        return;
    }
    
    chaseTimer = 0.f;
    
    // if in radius -> stop and shoot
    if(!enemy->flee)
    {   
        if(distance <= 360.f)
        {
            enemy->chase = false;
        }
        if(!enemy->chase)
        {
            if(distance >= 200.f)
            {
                if(attCooldown == 0.f){     // if cooldown is off -> shoot proyectile
                    EntityMng::spawnProyectile(proyectile, enemy->getWorldPos(), velocity, target);
                    attCooldown += deltaTime;
                } else if (attCooldown >= 0.8f) attCooldown = 0.f;
                else attCooldown += deltaTime;
                
                enemy->setDrawColor(BLUE);
            }
            else {
                // if too close -> get away
                enemy->flee = true;
            }
            velocity = {};
        }
    }

    EntityMng::tickProyectile(proyectile, deltaTime);
}
void shootTargetC(Enemy* enemy, Character* target, const float& deltaTime)
{
    if(enemy->neutral) return;

    Vector2& velocity = enemy->getVelocity();
    float& attCooldown = enemy->attackTimer;
    GenEntity*& proyectile = enemy->proyectile;
    float& fleeTimer = enemy->fleeTimer;
    float& chaseTimer = enemy->getRadiusEtc(2);

    // get target direction
    velocity = Vector2Subtract(target->getScreenPos(), enemy->getScreenPos());
    float distance = Vector2Length(velocity);       // distance from enemy to target

    if(enemy->flee){
        velocity = Vector2Scale(velocity, -1.f);
        fleeTimer += deltaTime;
        if(fleeTimer >= 0.7f && distance > 380.f) {fleeTimer = 0.f; enemy->flee = false;}
    }
        
    if(distance > 430.f)
    {
        enemy->chase = true;
        EntityMng::tickProyectile(proyectile, deltaTime);
        
        chaseTimer += deltaTime;
        if(chaseTimer < 0.2f) velocity = {};        // wait a bit before chasing
        else if (chaseTimer >= 5.f){                // become neutral after 5 sec chasing
            velocity = {};
            enemy->neutral = true;
        }

        return;
    }
    
    chaseTimer = 0.f;
    
    // if in radius -> stop and shoot
    if(!enemy->flee)
    {   
        if(distance <= 360.f)
        {
            enemy->chase = false;
        }
        if(!enemy->chase)
        {
            if(distance >= 200.f)
            {
                if(attCooldown == 0.f){     // if cooldown is off -> shoot proyectile
                    EntityMng::spawnProyectile(proyectile, enemy->getWorldPos(), velocity, target);
                    attCooldown += deltaTime;
                } else if (attCooldown >= 0.8f) attCooldown = 0.f;
                else attCooldown += deltaTime;
                
                enemy->setDrawColor(BLUE);
            }
            else {
                // if too close -> get away
                enemy->flee = true;
            }
            velocity = {};
        }
    }

    EntityMng::tickProyectile(proyectile, deltaTime);
}

const enemyData SLIME_ENEMYDATA{
    &Tex::texture_slime_idle,
    &Tex::texture_slime_run,
    6,
    1,
    4.f,
    40.f,
    5.f,
    300.f,
    &HEART_ITEMDATA,
    fleeTarget
};
const enemyData MADKNIGHT_ENEMYDATA{
    .texture_idle = &Tex::texture_knight_idle,
    .texture_run = &Tex::texture_knight_run,
    .maxFrames = 6,
    .frameRows = 1,
    .speed = 9.f,
    .health = 120.f,
    .damage = 10.f,
    .chase_radius = 400.f,
    .item_drop = &COIN_ITEMDATA,
    .behave = shootTargetC,
    .isNeutral = true
};
const enemyData RED_ENEMYDATA{
    &Tex::texture_red_idle,
    &Tex::texture_red_run,
    4,
    2,
    9.f,
    120.f,
    10.f,
    400.f,
    &GEM_ITEMDATA,
    shootTargetA
};
const enemyData* ENEMYDATA_ARR[]{
    &SLIME_ENEMYDATA,
    &MADKNIGHT_ENEMYDATA,
    &RED_ENEMYDATA
};

#endif