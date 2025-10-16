#include "GenericEntity.h"
#include "raymath.h"

GenEntity::GenEntity(){
    setAlive(false);
}
GenEntity::GenEntity( Vector2 pos, Vector2 direction, Character* playerPtr ) :
    worldPos(pos),
    velocity(direction),
    player(playerPtr)
{
    frameWidth = texture->width / maxFrames;
    frameHeight = texture->height;
    scale = 4.f;
}

bool GenEntity::tick(float deltaTime)
{
    if( !getAlive() ) return false;

    // MOVEMENT
    if(Vector2Length(velocity) != 0.0)
    {   
        movement = Vector2Scale(Vector2Normalize(velocity), speed);
        worldPos = Vector2Add(worldPos, movement);
        moveTimer += deltaTime;
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

    // check collision
    if( CheckCollisionRecs(getCollisionRec(), player->getHurtRec()) ){
        player->addHealth(10);
        // active = false;
        setAlive(false);
    }

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

void GenEntity::spawnReset(Vector2 pos, Vector2 direction, Character* playerPtr){
    worldPos = pos;
    velocity = direction;
    player = playerPtr;

    frameWidth = texture->width / maxFrames;
    frameHeight = texture->height;
    scale = 4.f;

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