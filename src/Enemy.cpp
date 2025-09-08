#include "Enemy.h"
// #include "raymath.h"
#include "EntityManager.h"
#include "EnemyData.h"
#include <iostream>

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
    if(enemy_data->isNeutral) neutral = true;
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
    if(!neutral){
        if(CheckCollisionRecs(target->getCollisionRec(),getCollisionRec())){
            target->takeDamage(damagePerSec * deltaTime);
        }
    }

    // ====== TAKE DAMAGE ============      (MAKE IT A FUNCTION!!!)
    if(target->getIsAttack()) {
        if(CheckCollisionRecs( getCollisionRec(), target->getWeaponCollisionRec() )) {
            takeDamage(target->getDamage());
        }
    }
    
    // draw health bar
    Rectangle healthBar{getScreenPos().x, getScreenPos().y, health, 5.f};
    if(data != nullptr){
        DrawRectangle(healthBar.x, healthBar.y, data->health, healthBar.height, BLACK);
    }
    DrawRectangle(healthBar.x, healthBar.y, healthBar.width, healthBar.height, GREEN);

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