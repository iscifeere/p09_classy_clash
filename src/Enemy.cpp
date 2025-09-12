#include "Enemy.h"
// #include "raymath.h"
#include "EntityManager.h"
#include "EnemyData.h"
#include <iostream>

Enemy::Enemy(){
    // action = chaseTarget;
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

    // action = chaseTarget;
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
    neutral = enemy_data->isNeutral;

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

    // action = chaseTarget;
}

bool Enemy::tick(float deltaTime){

    if (!getAlive()) return false;    // if not alive, do nothing and return false

    // ====== MOVEMENT ============
    velocity = {};          // reset velocity

    action(this, target, deltaTime);

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

    // KILL WITH CURSOR
    if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
        if(CheckCollisionRecs(Rectangle{GetMousePosition().x - 5, GetMousePosition().y - 5, 5, 5}, getCollisionRec())){
            setAlive(false);
        }
    }
    
    drawHealthBar();

    return true;
}

Vector2 Enemy::getScreenPos(){
    return Vector2Subtract(
        Vector2{worldPos.x - width*scale*0.5f, worldPos.y - height*scale*0.5f}, 
        Vector2Subtract(target->getWorldPos(), 
        Vector2{static_cast<float>(Tex::winSize[0])*0.5f,static_cast<float>(Tex::winSize[1])*0.5f}));
}

void Enemy::takeDamage(float damage){
    BaseCharacter::takeDamage(damage);
    neutral = false;
}

void Enemy::deathSequence(){
    setAlive(false);
    EntityMng::spawnItem(worldPos, target, itemDrop);
}

float& Enemy::getRadiusEtc(int choice)
{
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

void Enemy::showDebugData()     // draw debug data
{
    DrawRectangleLines(getScreenPos().x, getScreenPos().y, scale*width, scale*height, YELLOW);
    DrawText(TextFormat("%01.01f",health), getScreenPos().x, getScreenPos().y, 20, WHITE);
    DrawText(TextFormat("%01.01f",getWorldPos().x), getScreenPos().x, getScreenPos().y + height*scale - 20, 10, WHITE);
    DrawText(TextFormat("%01.01f",getWorldPos().y), getScreenPos().x, getScreenPos().y + height*scale - 10, 10, WHITE);
    DrawText(TextFormat("%01.01f",chaseTime), getScreenPos().x, getScreenPos().y + height*scale, 10, WHITE);
}

void Enemy::drawHealthBar()
{
    Rectangle healthBar{getScreenPos().x, getScreenPos().y, health, 5.f};
    if(data != nullptr){
        DrawRectangle(healthBar.x, healthBar.y, data->health, healthBar.height, BLACK);
    }
    DrawRectangle(healthBar.x, healthBar.y, healthBar.width, healthBar.height, GREEN);
}