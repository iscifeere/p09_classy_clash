#include "Item.h"
#include "raymath.h"
#include <cmath>

Item::Item(){
    scale = 4.f;
}

Item::Item(Vector2 pos, Character* player_ptr ) :
    worldPos(pos),
    player(player_ptr)
{
    frameWidth = texture->width / maxFrames;
    frameHeight = texture->height;
    scale = 4.f;
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
    scale = 4.f;
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

    // // draw item
    // Rectangle source{frameWidth*frame, 0.f, frameWidth, frameHeight};
    // Rectangle dest{getScreenPos().x, getScreenPos().y, frameWidth * scale, frameHeight * scale};
    // DrawTexturePro(*texture, source, dest, Vector2{0.f,0.f}, 0.f, WHITE);
    // // DrawText(TextFormat("%02.02f",moveTimer), screenPos.x, screenPos.y, 10, WHITE);

    // KILL WITH CURSOR
    if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
        if(CheckCollisionRecs(Rectangle{GetMousePosition().x - 5, GetMousePosition().y - 5, 5, 5}, getCollisionRec())){
            setAlive(false);
        }
    }

    return true;
}

Vector2 Item::getScreenPos(){
    return Vector2Subtract(
        Vector2{worldPos.x - frameWidth*scale*0.5f, worldPos.y - frameHeight*scale*0.5f}, 
        Vector2Subtract(player->getWorldPos(), 
        Tex::halfWinSize) );
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

void Item::showDebugData(){
    DrawRectangleLines( getScreenPos().x, getScreenPos().y, frameWidth*scale, frameHeight*scale, YELLOW );
    DrawText( TextFormat("%01.01f",getWorldPos().x), getScreenPos().x, getScreenPos().y + frameHeight*scale - 20, 10, WHITE );
    DrawText( TextFormat("%01.01f",getWorldPos().y), getScreenPos().x, getScreenPos().y + frameHeight*scale - 10, 10, WHITE );
}

void Item::render(){
    // draw item
    Rectangle source{frameWidth*frame, 0.f, frameWidth, frameHeight};
    Rectangle dest{getScreenPos().x, getScreenPos().y, frameWidth * scale, frameHeight * scale};
    DrawTexturePro(*texture, source, dest, Vector2{0.f,0.f}, 0.f, WHITE);

    // DrawText(TextFormat("%02.02f",moveTimer), screenPos.x, screenPos.y, 10, WHITE);
}