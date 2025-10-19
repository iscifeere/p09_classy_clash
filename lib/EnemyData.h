#ifndef ENEMYDATA_H
#define ENEMYDATA_H

#include "Enemy.h"
#include "EntityManager.h"

struct enemyData{
    Texture2D* texture_idle{&Tex::texture_goblin_idle};
    Texture2D* texture_run{&Tex::texture_goblin_run};
    int maxFramesIdle{6};
    int maxFramesRun{6};
    int frameRows{1};
    bool ignoreFrameRows{false};

    float speed{};
    float health{};
    float damage{};
    float chase_radius{};
    bool isNeutral{false};

    // Location and size of collisionBox & hurtBox.
    // x and y are propotional displacement from sprite screen position,
    // width and height are scaling proportional to texture.
    Rectangle collisionBox{
        .x = 0.f,
        .y = 0.f,
        .width = 1.f,
        .height = 1.f,
    };
    Rectangle hurtBox{
        .x = 0.f,
        .y = 0.f,
        .width = 1.f,
        .height = 1.f,
    };

    const itemData* item_drop{&HEART_ITEMDATA};
    void(*behave)(Enemy* enemy, Character* player, const float& deltaTime);
};


// ========= ENEMY BEHAVE FUNCTIONS =========================

inline void chaseTarget(Enemy* enemy, Character* target, const float& deltaTime)
{
    Vector2& velocity = enemy->getVelocity();
    float& in_radius = enemy->getRadiusEtc(0);
    float& out_radius = enemy->getRadiusEtc(1);
    float& chaseTime = enemy->getRadiusEtc(2);

    // get to target
    velocity = Vector2Subtract(target->getScreenPos(), enemy->getScreenPos());
    float distance = Vector2Length(velocity);
    // if too close / too far -> don't chase
    if(distance < in_radius || distance > out_radius ){
        velocity = {};
        chaseTime = 0.f;
        return;
    }

    chaseTime += deltaTime;
    if(chaseTime < 0.2f || chaseTime >= 5.f) velocity = {}; // wait a bit to move, and at 5 sec stop
}

inline void fleeTarget(Enemy* enemy, Character* target, const float& deltaTime)
{
    Vector2& velocity = enemy->getVelocity();
    float& out_radius = enemy->getRadiusEtc(1);
    float& chaseTime = enemy->getRadiusEtc(2);
    
    // get inverted target direction
    velocity = Vector2Subtract(enemy->getScreenPos(), target->getScreenPos());
    float distance = Vector2Length(velocity);

    // if too far don't flee
    if(distance > out_radius ){
      velocity = {};
      chaseTime = 0.f;
      return;
    }

    chaseTime += deltaTime;
    if(chaseTime < 0.2f || chaseTime >= 5.f) velocity = {}; // wait a bit to move, and at 5 sec stop
}

inline void shootTarget(Enemy* enemy, Character* target, const float& deltaTime)
{
    if(enemy->neutral) return;

    Vector2& velocity = enemy->getVelocity();
    float& attCooldown = enemy->attackTimer;
    float& fleeTimer = enemy->fleeTimer;
    float& chaseTimer = enemy->getRadiusEtc(2);

    // get target direction
    velocity = Vector2Subtract(target->getScreenPos(), enemy->getScreenPos());
    // velocity = Vector2{
    //     .x = target->getCollisionRec().x - enemy->getCollisionRec().x,
    //     .y = target->getCollisionRec().y - enemy->getCollisionRec().y
    // };
    float distance = Vector2Length(velocity);       // distance from enemy to target

    if(enemy->flee){
        velocity = Vector2Scale(velocity, -1.f);
        fleeTimer += deltaTime;
        if(fleeTimer >= 0.7f && distance > 380.f) {fleeTimer = 0.f; enemy->flee = false;}
    }
        
    if(distance > 430.f)
    {
        enemy->chase = true;
        
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
            if(distance >= 215.f)
            {
                if(attCooldown == 0.f){     // if cooldown is off -> shoot proyectile
                    EntityMng::spawnProyectile(enemy->getWorldPos(), velocity, true);
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
    
}


// ========= ENEMY DATA STRUCTS ============================

const enemyData DEFAULT_ENEMYDATA{
    .behave = chaseTarget
};
const enemyData SLIME_ENEMYDATA{
    .texture_idle = &Tex::texture_slime_idle,
    .texture_run = &Tex::texture_slime_run,
    .maxFramesIdle = 6,
    .maxFramesRun = 6,
    .frameRows = 1,
    .speed = 4.f,
    .health = 40.f,
    .damage = 5.f,
    .chase_radius = 300.f,
    .item_drop = &HEART_ITEMDATA,
    .behave = fleeTarget
};
const enemyData MADKNIGHT_ENEMYDATA{
    .texture_idle = &Tex::texture_knight_idle,
    .texture_run = &Tex::texture_knight_run,
    .maxFramesIdle = 6,
    .maxFramesRun = 6,
    .frameRows = 1,
    .speed = 9.f,
    .health = 120.f,
    .damage = 10.f,
    .chase_radius = 400.f,
    .isNeutral = true,
    .collisionBox = {
        .x = 0.2f,
        .y = 0.75f,
        .width = 0.6f,
        .height = 0.25f
    },
    .item_drop = &COIN_ITEMDATA,
    .behave = shootTarget
};
const enemyData RED_ENEMYDATA{
    .texture_idle = &Tex::texture_red_idle,
    .texture_run = &Tex::texture_red_run,
    .maxFramesIdle = 4,
    .maxFramesRun = 6,
    .frameRows = 2,
    .ignoreFrameRows = true,
    .speed = 9.f,
    .health = 120.f,
    .damage = 10.f,
    .chase_radius = 400.f,
    .collisionBox = {
        .x = 0.25f,
        .y = 0.25f,
        .width = 0.5f,
        .height = 0.5f
    },
    .item_drop = &GEM_ITEMDATA,
    .behave = chaseTarget
};
const enemyData* ENEMYDATA_ARR[]{
    &SLIME_ENEMYDATA,
    &MADKNIGHT_ENEMYDATA,
    &RED_ENEMYDATA
};

#endif