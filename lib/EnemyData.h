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
    int enemyType{};
    bool isNeutral{false};  // unused

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
    velocity = Vector2Subtract(target->getWorldPos(), enemy->getWorldPos());
    float distance = Vector2Length(velocity);

    // damage player on contact
    if(CheckCollisionRecs(target->getHurtRec(),enemy->getHurtRec()))
    {
        target->takeDamage(enemy->getDamage() * deltaTime);
    }

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
    float& out_radius = enemy->getRadiusEtc(1); // unused
    float& chaseTime = enemy->getRadiusEtc(2);
    
    // get inverted target direction
    velocity = Vector2Subtract(enemy->getWorldPos(), target->getWorldPos());
    float distance = Vector2Length(velocity);

    // if too far don't flee
    if(distance > 600.f){
      velocity = {};
      chaseTime = 0.f;
      enemy->setEnemyState(0);  // set state to idle
      return;
    }

    chaseTime += deltaTime;
    
    if(chaseTime < 0.2f) velocity = {}; // wait a bit to start moving
    else if (chaseTime >= 5.f){ // after 5 sec return to idle
        velocity = {};
        chaseTime = 0.f;
        enemy->setEnemyState(0);  // set state to idle
        return;
    }
}

inline void shootTarget(Enemy* this_enemy, Character* target, const float& deltaTime)
{
    Vector2& velocity = this_enemy->getVelocity();
    float& attCooldown = this_enemy->attackTimer;
    float& fleeTimer = this_enemy->fleeTimer;
    float& chaseTimer = this_enemy->getRadiusEtc(2);
    float& askNearestEnemyTimer = this_enemy->freeUseTimer1;
    Enemy*& nearestEnemy = this_enemy->nearestEnemy;

    // get target direction
    Vector2 vecToPlayer = Vector2Subtract(target->getWorldPos(), this_enemy->getWorldPos());
    float distanceToPlayer = Vector2Length(vecToPlayer);
    velocity = vecToPlayer;
    
    // get direction of nearest enemy of same type
    Vector2 vecAwayFromNearestEnemy{};
    float distanceToNearestEnemy{};

    if(askNearestEnemyTimer == 0.f)
    {
        nearestEnemy = EntityMng::getNearestChasingEnemyByType(this_enemy);    // expensive function, shouldn't be done every frame
        askNearestEnemyTimer += deltaTime;
    }
    else if(askNearestEnemyTimer >= 0.7f) askNearestEnemyTimer = 0.f;
    else askNearestEnemyTimer += deltaTime;

    if(nearestEnemy != nullptr)
    {
        vecAwayFromNearestEnemy = Vector2Subtract(this_enemy->getWorldPos(), nearestEnemy->getWorldPos());
        distanceToNearestEnemy = Vector2Length(vecAwayFromNearestEnemy);
    }

    if(this_enemy->flee)
    {
        velocity = Vector2Scale(vecToPlayer, -1.f);

        fleeTimer += deltaTime;
        if(fleeTimer >= 0.7f && distanceToPlayer > 380.f) {fleeTimer = 0.f; this_enemy->flee = false;}
    }
    
    // get away from near enemy
    if(distanceToNearestEnemy <= 200.f)     // if too close to nearest enemy, prioritize them
    {
        velocity = Vector2Add( Vector2Scale(velocity, 0.2f), Vector2Scale(vecAwayFromNearestEnemy, 0.8f) );
    }
    else if(distanceToNearestEnemy <= 400.f)    // if nearest enemy and player are equally distant, get away from both
    {
        velocity = Vector2Add( Vector2Scale(velocity, 0.5f), Vector2Scale(vecAwayFromNearestEnemy, 0.5f) );
    }
        
    if(distanceToPlayer > 430.f)
    {
        this_enemy->chase = true;
        
        chaseTimer += deltaTime;
        if(chaseTimer < 0.2f) velocity = {};        // wait a bit before chasing
        else if (chaseTimer >= 7.f){                // become neutral after 5 sec chasing
            velocity = {};
            this_enemy->neutral = true;
            this_enemy->setEnemyState(0);    // set state to idle
            chaseTimer = 0.f;
        }

        return;
    }
    
    chaseTimer = 0.f;
    
    // if in radius -> stop and shoot
    if(!this_enemy->flee)
    {   
        if(distanceToPlayer <= 360.f)
        {
            this_enemy->chase = false;
        }
        if(!this_enemy->chase)
        {
            if(distanceToPlayer >= 215.f)
            {
                if(attCooldown == 0.f){     // if cooldown is off -> shoot proyectile
                    // velocity = vecToPlayer;
                    EntityMng::spawnProyectile(this_enemy->getWorldPos(), vecToPlayer, true);
                    attCooldown += deltaTime;
                } else if (attCooldown >= 0.8f) attCooldown = 0.f;
                else attCooldown += deltaTime;
                
                this_enemy->setDrawColor(BLUE);
            }
            else {
                // if too close -> get away
                this_enemy->flee = true;
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
    .enemyType = 0,
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
    .enemyType = 1,
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
    .enemyType = 2,
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