#include "GenericEntity.h"
#include "raymath.h"
#include "EntityManager.h"

void GenEntity::init(){
    scale = 4.f;
    frameWidth = texture->width / maxFrames;
    frameHeight = texture->height;
}

GenEntity::GenEntity(){
    setAlive(false);
    player = EntityMng::getPlayerPtr();
    init();
}

GenEntity::GenEntity( Vector2 pos, Vector2 direction) :
    worldPos(pos),
    velocity(direction)
{
    player = EntityMng::getPlayerPtr();
    init();
}

bool GenEntity::tick(float deltaTime)
{
    if( !getAlive() ) return false;

    // MOVEMENT
    movement = Vector2Scale(Vector2Normalize(velocity), speed);
    worldPos = Vector2Add(worldPos, movement);
    moveTimer += deltaTime;
    if( moveTimer >= 0.7f ) setAlive(false);

    return true;
}

Vector2 GenEntity::getScreenPos(){
    return Vector2Subtract(worldPos, player->getWindowOriginWorPos());
}

Rectangle GenEntity::getCollisionRec(){
    Vector2 renderPos{getRenderPos()};
    float scaledWidth = frameWidth*scale;
    float scaledHeight = frameHeight*scale;

    return Rectangle{
        renderPos.x + scaledWidth * 0.25f,
        renderPos.y + scaledHeight * 0.25f,
        scaledWidth * 0.5f,
        scaledHeight * 0.5f
    };
}

void GenEntity::spawnReset(Vector2 pos, Vector2 direction, bool isEnemy){
    worldPos = pos;
    velocity = direction;
    init();
    setIsEnemy(isEnemy);

    moveTimer = 0.f;
    setAlive(true);
}

void GenEntity::showDebugData(){
    Rectangle collisionRec{getCollisionRec()};
    DrawRectangleLines( collisionRec.x, collisionRec.y, collisionRec.width, collisionRec.height, YELLOW );
}

void GenEntity::render(){
    Vector2 renderPos{getRenderPos()};
    DrawTextureEx(*texture, renderPos, 0.f, scale, drawColor);
}

void GenEntity::checkPlayerCollision(){
    if( CheckCollisionRecs(getCollisionRec(), player->getHurtRec()) ){
            // player->addHealth(10);
            player->takeDamage(10);
            setAlive(false);
        }
}