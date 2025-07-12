#include "Item.h"
#include "raymath.h"
#include <cmath>

Item::Item(){

}

Item::Item(Vector2 pos, Character* player_ptr ) :
    worldPos(pos),
    player(player_ptr)
{
    frameWidth = texture->width / maxFrames;
    frameHeight = texture->height;
}

Item::Item(Vector2 pos, Character* player_ptr, const itemData* item_data ) :
    worldPos(pos),
    player(player_ptr)
{
    data = item_data;

    texture = data->texture;
    maxFrames = data->maxFrames;
    updateTime = data->updateTime;
    item_effect = data->effect;

    frameWidth = texture->width / maxFrames;
    frameHeight = texture->height;
}

bool Item::tick(float deltaTime){

    if( !getAlive() ) return false;

    // update animation
    runningTime += deltaTime;
    if(runningTime >= updateTime){
        frame++;
        runningTime = 0.f;
        if(frame >= maxFrames) frame = 0;
    }

    // move item
    moveTimer += deltaTime;
    if(moveTimer >= 10.f) moveTimer = 0.f;

    float degrees{ (180 * moveTimer)/180 * PI };    // in radians
    worldPos.y += sin(degrees) * 0.8f;

    // collision w/ player
    if( player != nullptr ){
        if( CheckCollisionRecs( getCollisionRec(), player->getCollisionRec() ) ){
            // execute [EFFECT] on player...
            // player->addHealth(20.f);
            // data->effect(player);
            item_effect(player);
            setAlive(false);
        }
    }

    // draw item
    Rectangle source{frameWidth*frame, 0.f, frameWidth, frameHeight};
    Rectangle dest{getScreenPos().x, getScreenPos().y, frameWidth * scale, frameHeight * scale};
    DrawTexturePro(*texture, source, dest, Vector2{0.f,0.f}, 0.f, WHITE);

    // DrawText(TextFormat("%02.02f",moveTimer), screenPos.x, screenPos.y, 10, WHITE);
    return true;
}

Vector2 Item::getScreenPos(){
    return Vector2{Vector2Subtract( worldPos, player->getWorldPos() )};
}

Rectangle Item::getCollisionRec()       // necesita revisión
{
    Vector2 charPos{ getScreenPos() };
    return Rectangle{
        charPos.x,
        charPos.y,
        Tex::texture_heart.width * scale,
        Tex::texture_heart.height * scale
    };
}