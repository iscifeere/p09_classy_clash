#include "Enemy.h"
// #include "raymath.h"
#include "EntityManager.h"
#include <iostream>

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
    &Tex::texture_knight_idle,
    &Tex::texture_knight_run,
    6,
    1,
    9.f,
    120.f,
    10.f,
    400.f,
    &COIN_ITEMDATA,
    shootTargetC
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

Enemy::Enemy(){
    
}

Enemy::Enemy(Vector2 pos)
{
    worldPos = pos;
    texture = &Tex::texture_goblin_idle;
    idle = &Tex::texture_goblin_idle;
    run = &Tex::texture_goblin_run;

    width = texture->width / maxFrames;
    height = texture->height;

    speed = 5.f;   // default speed
    health = 60;    // default health
}

Enemy::Enemy(Vector2 pos, const enemyData* enemy_data)
{
    worldPos = pos;
    data = enemy_data;

    texture = enemy_data->texture_idle;
    idle = enemy_data->texture_idle;
    run = enemy_data->texture_run;
    maxFrames = enemy_data->maxFrames;
    itemDrop = enemy_data->item_drop;

    width = texture->width / maxFrames;
    height = texture->height / enemy_data->frameRows;

    speed = enemy_data->speed;
    health = enemy_data->health;
    damagePerSec = enemy_data->damage;
    chaseRadius = enemy_data->chase_radius;

    action = enemy_data->behave;
}

Enemy::Enemy(Vector2 pos, Texture2D* idle_texture, Texture2D* run_texture)
{
    worldPos = pos;
    texture = idle_texture;
    idle = idle_texture;
    run = run_texture;

    width = texture->width / maxFrames;
    height = texture->height;

    speed = 5.f;   // default speed
    health = 60;    // default health
}

bool Enemy::tick(float deltaTime){

    if (!getAlive()) return false;    // if not alive, do nothing and return false

    // ====== MOVEMENT ============
    velocity = {};          // reset velocity
    // get to target
    // velocity = Vector2Subtract(target->getScreenPos(), getScreenPos());

    action(this, target, deltaTime);

    // // if too close / too far -> don't chase
    // if(Vector2Length(velocity) < radius || Vector2Length(velocity) > chaseRadius ) velocity = {};

    // if(Vector2Length(velocity) > 0.f) chaseTime += deltaTime;
    // else chaseTime = 0.f;

    // if(chaseTime >= 5.f) velocity = {}; // at 5 sec chaseTime stop chasing


    // ====== TICK AND VARIABLE RESETS ============
    BaseCharacter::tick(deltaTime);
    
    if(!invul) drawColor = WHITE; // reset drawColor if not invulnerable
    else { 
        hurtTime += deltaTime;
        if(hurtTime >= 0.5f) { hurtTime = 0.f; invul = false; }
    }


    // ====== DAMAGE TARGET ON CONTACT ============
    if(CheckCollisionRecs(target->getCollisionRec(),getCollisionRec())){
        target->takeDamage(damagePerSec * deltaTime);
    }

    // ====== TAKE DAMAGE ============      (MAKE IT A FUNCTION!!!)
    if(target->getIsAttack()) {
        if(CheckCollisionRecs( getCollisionRec(), target->getWeaponCollisionRec() )) {
            takeDamage(target->getDamage());
        }
    }

    // draw debug text
    DrawText(TextFormat("%01.02f",health), getScreenPos().x, getScreenPos().y, 20, WHITE);
    DrawText(TextFormat("%01.02f",chaseTime), getScreenPos().x, getScreenPos().y + height*scale, 10, WHITE);

    return true;
}

Vector2 Enemy::getScreenPos(){
    return Vector2Subtract(worldPos, target->getWorldPos());
}

void Enemy::takeDamage(float damage)
{
    BaseCharacter::takeDamage(damage);
    neutral = false;
};

void Enemy::deathSequence(){
    setAlive(false);
    EntityMng::spawnItem(worldPos, target, itemDrop);
}

float& Enemy::getRadiusEtc(int choice){
    switch (choice)
    {
    case 0:
        return radius;
        break;

    case 1:
        return chaseRadius;
        break;

    case 2:
        return chaseTime;
        break;
    
    default:
        return radius;
        break;
    }
}