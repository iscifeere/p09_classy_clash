#include "GenericEntity.h"
#include "raymath.h"

GenEntity::GenEntity( Vector2 pos, Character* playerPtr ) :
    worldPos(pos),
    player(playerPtr)
{
    frameWidth = texture->width / maxFrames;
    frameHeight = texture->height;
}
GenEntity::GenEntity( Vector2 pos, Vector2 direction, Character* playerPtr ) :
    worldPos(pos),
    velocity(direction),
    player(playerPtr)
{
    frameWidth = texture->width / maxFrames;
    frameHeight = texture->height;
}

bool GenEntity::tick(float deltaTime)
{
    if( !getAlive() ) return false;

    // drawColor = BLUE;       // reset

    // MOVEMENT
    if(Vector2Length(velocity) != 0.0)
    {   
        movement = Vector2Scale(Vector2Normalize(velocity), speed);
        worldPos = Vector2Add(worldPos, movement);
        moveTimer += deltaTime;
        // if(Vector2Length( getScreenPos() ) > 600.f) {velocity = {}; drawColor = RED; }
        drawColor = RED;
        if( moveTimer >= 0.7f ) setAlive(false);
    } else
    {
        velocity = {};
        moveTimer = 0.f;
        drawColor = BLUE;

        if(IsKeyPressed(KEY_L)) velocity.x += 1.f;
        if(IsKeyPressed(KEY_J)) velocity.x -= 1.f;
        if(IsKeyPressed(KEY_I)) velocity.y -= 1.f;
        if(IsKeyPressed(KEY_K)) velocity.y += 1.f;
    }

    screenPos = getScreenPos();

    // check collision
    if( CheckCollisionRecs(getCollisionRec(), player->getCollisionRec()) ){
        player->addHealth(10);
        // active = false;
        setAlive(false);
    }

    // DRAW ENTITY
    DrawTextureEx(*texture, screenPos, 0.f, scale, drawColor);

    return true;
}

// Vector2 GenEntity::getScreenPos()
// {
//     return Vector2{Vector2Subtract( worldPos, player->getWorldPos() )};
// }
Vector2 GenEntity::getScreenPos(){
    return Vector2Subtract(
        Vector2{worldPos.x - frameWidth*scale*0.5f, worldPos.y - frameHeight*scale*0.5f}, 
        Vector2Subtract(player->getWorldPos(), 
        Tex::halfWinSize) );
}

Rectangle GenEntity::getCollisionRec()
{
    Vector2 screenPos{ getScreenPos() };
    return Rectangle{
        screenPos.x,
        screenPos.y,
        frameWidth,
        frameHeight
    };
}

void GenEntity::spawnReset()
{
    moveTimer = 0.f;
    velocity = {};
    // active = true;
    setAlive(true);
}
void GenEntity::spawnReset(Vector2 pos, Vector2 direction)
{
    moveTimer = 0.f;
    velocity = direction;
    worldPos = pos;
    // active = true;
    setAlive(true);
}

void GenEntity::showDebugData(){
    DrawRectangleLines( getScreenPos().x, getScreenPos().y, frameWidth*scale, frameHeight*scale, YELLOW );
}