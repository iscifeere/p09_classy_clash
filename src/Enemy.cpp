#include "Enemy.h"
// #include "raymath.h"
#include "EntityManager.h"
#include "EnemyData.h"
#include <iostream>

void Enemy::init(){
    std::cout << "[Enemy init function (" << this << ") ]" << std::endl;
    
    scale = 8.f;

    texture = data->texture_idle;
    idle = data->texture_idle;
    run = data->texture_run;
    
    maxFramesIdle = data->maxFramesIdle;
    maxFramesRun = data->maxFramesRun;
    maxFrames = maxFramesIdle;
    maxFrameRows = data->frameRows;
    ignoreFrameRows = data->ignoreFrameRows;

    frameWidth = texture->width / maxFrames;
    frameHeight = texture->height / maxFrameRows;
    
    speed = data->speed;
    health = data->health;
    chaseRadius = data->chase_radius;
    itemDrop = data->item_drop;
    idleLogic = data->idleLogic;
    transitionLogic = data->transitionLogic;
    actionLogic = data->actionLogic;
    currentStateLogic = data->idleLogic;
}

Enemy::Enemy(){
    std::cout << "\n[Enemy default constructor (" << this << ") ]" << std::endl;
    setAlive(false);
    target = EntityMng::getPlayerPtr();
    data = &DEFAULT_ENEMYDATA;
    init();
}

void Enemy::spawnReset(Vector2 pos, const enemyData* enemy_data)
{
    worldPos = pos;
    data = enemy_data;
    init();

    // resetting state variables
    drawColor = WHITE;
    hurtTime = 0.f;
    invul = false;
    wanderingPoint = {};
    state = EnemyState::IDLE;

    setAlive(true);
}

bool Enemy::tick(float deltaTime){

    if (!getAlive()) return false;    // if not alive, do nothing and return false

    // ====== MOVEMENT ============
    currentStateLogic(this, target, deltaTime);

    // ====== TICK AND VARIABLE RESETS ============
    BaseCharacter::tick(deltaTime);

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
    setEnemyState(EnemyState::ACTION);
}

void Enemy::deathSequence(){
    setAlive(false);
    EntityMng::spawnItem(worldPos, itemDrop);
    target->incrementKilledEnemies();
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

    // KILL WITH CURSOR
    if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        if(CheckCollisionRecs(Rectangle{GetMousePosition().x - 5, GetMousePosition().y - 5, 5, 5}, getHurtRec()))
        {
            setAlive(false);
        }
    }
    
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

    Vector2 screenPos{ getScreenPos() };
    switch (state)
    {
    case EnemyState::PLAYER_SPOTTED:
        DrawText("!", static_cast<int>(screenPos.x), static_cast<int>(screenPos.y) - 90, 45, WHITE);
        break;

    case EnemyState::ACTION:
        drawHealthBar();
        break;
    
    default:
        break;
    }

    drawColor = WHITE;    // reset color
}

EnemyType Enemy::getEnemyType()
{
    return data->enemyType;
}

float Enemy::getDamage()
{
    return data->damage;
}

void Enemy::setEnemyState(EnemyState newState)
{
    state = newState;
    switch (newState)
    {
    case EnemyState::IDLE:
        currentStateLogic = idleLogic;
        break;

    case EnemyState::PLAYER_SPOTTED:
        currentStateLogic = transitionLogic;
        break;

    case EnemyState::ACTION:
        currentStateLogic = actionLogic;
        break;
    
    default:
        break;
    }
}