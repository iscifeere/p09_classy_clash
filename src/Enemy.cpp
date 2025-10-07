#include "Enemy.h"
// #include "raymath.h"
#include "EntityManager.h"
#include "EnemyData.h"
#include <iostream>

Enemy::Enemy(){
    // action = chaseTarget;
    scale = 8.0f;
    data = &DEFAULT_ENEMYDATA;
}

Enemy::Enemy(Vector2 pos)
{
    worldPos = pos;
    texture = &Tex::texture_goblin_idle;
    idle = &Tex::texture_goblin_idle;
    run = &Tex::texture_goblin_run;

    frameWidth = texture->width / maxFrames;
    frameHeight = texture->height;
    scale = 8.0f;

    speed = 5.f;   // default speed
    health = 60;    // default health

    // action = chaseTarget;
    data = &DEFAULT_ENEMYDATA;
}

Enemy::Enemy(Vector2 pos, const enemyData* enemy_data)
{
    worldPos = pos;
    data = enemy_data;

    texture = data->texture_idle;
    idle = data->texture_idle;
    run = data->texture_run;
    maxFramesIdle = data->maxFramesIdle;
    maxFramesRun = data->maxFramesRun;
    maxFrames = maxFramesIdle;
    maxFrameRows = data->frameRows;
    ignoreFrameRows = data->ignoreFrameRows;
    itemDrop = data->item_drop;

    frameWidth = texture->width / maxFrames;
    frameHeight = texture->height / data->frameRows;
    scale = 8.0f;

    speed = data->speed;
    health = data->health;
    damagePerSec = data->damage;
    chaseRadius = data->chase_radius;
    neutral = data->isNeutral;

    action = data->behave;
}

Enemy::Enemy(Vector2 pos, Texture2D* idle_texture, Texture2D* run_texture)
{
    worldPos = pos;
    texture = idle_texture;
    idle = idle_texture;
    run = run_texture;

    frameWidth = texture->width / maxFrames;
    frameHeight = texture->height;
    scale = 8.0f;

    speed = 5.f;   // default speed
    health = 60;    // default health

    // action = chaseTarget;
    data = &DEFAULT_ENEMYDATA;
}

bool Enemy::tick(float deltaTime){

    if (!getAlive()) return false;    // if not alive, do nothing and return false

    // ====== MOVEMENT ============
    velocity = {};          // reset velocity

    action(this, target, deltaTime);

    // ====== TICK AND VARIABLE RESETS ============
    BaseCharacter::tick(deltaTime);
    
    if(invul){
        hurtTime += deltaTime;
        if(hurtTime >= 0.5f){
            hurtTime = 0.f;
            invul = false;
            drawColor = WHITE;
        } else drawColor = RED;
    }

    // ====== DAMAGE TARGET ON CONTACT ============
    if(!neutral){
        if(CheckCollisionRecs(target->getHurtRec(),getHurtRec())){
            target->takeDamage(damagePerSec * deltaTime);
        }
    }

    // ====== TAKE DAMAGE ============      (MAKE IT A FUNCTION!!!)
    if(target->getIsAttack()) {
        if(CheckCollisionRecs( getHurtRec(), target->getWeaponCollisionRec() )) {
            takeDamage(target->getDamage());
        }
    }

    // KILL WITH CURSOR
    if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
        if(CheckCollisionRecs(Rectangle{GetMousePosition().x - 5, GetMousePosition().y - 5, 5, 5}, getHurtRec())){
            setAlive(false);
        }
    }

    return true;
}

Vector2 Enemy::getScreenPos(){
    return Vector2Subtract(worldPos, target->getWindowOriginWorPos());
}

Rectangle Enemy::getCollisionRec(){
    Vector2 renderPos{getRenderPos()};
    float scaledWidth = frameWidth*scale;
    float scaledHeight = frameHeight*scale;

    return Rectangle{
        // displacement
        renderPos.x + ( scaledWidth * data->collisionBox.x ),
        renderPos.y + ( scaledHeight * data->collisionBox.y ),

        // scaling
        scaledWidth * data->collisionBox.width,
        scaledHeight * data->collisionBox.height
    };
}

Rectangle Enemy::getHurtRec(){
    Vector2 renderPos{getRenderPos()};
    float scaledWidth = frameWidth*scale;
    float scaledHeight = frameHeight*scale;

    return Rectangle{
        // displacement
        renderPos.x + ( scaledWidth * data->hurtBox.x ),
        renderPos.y + ( scaledHeight * data->hurtBox.y ),

        // scaling
        scaledWidth * data->hurtBox.width,
        scaledHeight * data->hurtBox.height
    };   
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
    Vector2 screenPos{getScreenPos()};
    Vector2 renderPos{getRenderPos(screenPos)};
    Rectangle collisionRec{getCollisionRec()};
    Rectangle hurtRec{getHurtRec()};
    float scaledWidth = frameWidth*scale;
    float scaledHeight = frameHeight*scale;

    DrawRectangleLines(hurtRec.x, hurtRec.y, hurtRec.width, hurtRec.height, RED);
    DrawRectangleLines(collisionRec.x, collisionRec.y, collisionRec.width, collisionRec.height, YELLOW);
    DrawCircleV(screenPos, 5.f, BLUE);  // worldPos mark
    DrawText(TextFormat("%01.01f",health), renderPos.x + 5, renderPos.y, 20, WHITE);
    DrawText(TextFormat("%01.01f",worldPos.x), collisionRec.x + 5, collisionRec.y + collisionRec.height - 20, 10, WHITE);
    DrawText(TextFormat("%01.01f",worldPos.y), collisionRec.x + 5, collisionRec.y + collisionRec.height - 10, 10, WHITE);
    DrawText(TextFormat("%01.01f",chaseTime), collisionRec.x + 5, collisionRec.y + collisionRec.height, 10, WHITE);
}

void Enemy::drawHealthBar()
{
    Vector2 renderPos{getRenderPos()};
    Rectangle healthBar{renderPos.x, renderPos.y, health, 5.f};
    if(data != nullptr){
        DrawRectangle(healthBar.x, healthBar.y, data->health, healthBar.height, BLACK);
    }
    DrawRectangle(healthBar.x, healthBar.y, healthBar.width, healthBar.height, GREEN);
}

void Enemy::render(){
    BaseCharacter::render();
    drawHealthBar();
    drawColor = WHITE;    // reset color
}