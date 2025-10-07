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
    return Vector2Subtract(worldPos, player->getWindowOriginWorPos());
}

Rectangle Item::getCollisionRec()
{
    Vector2 renderPos{getRenderPos()};
    return Rectangle{
        renderPos.x,
        renderPos.y,
        Tex::texture_heart.width * scale,
        Tex::texture_heart.height * scale
    };
}

void Item::showDebugData(){
    Vector2 screenPos{getScreenPos()};
    Vector2 renderPos{getRenderPos(screenPos)};
    float scaledWidth = frameWidth*scale;
    float scaledHeight = frameHeight*scale;

    DrawRectangleLines( renderPos.x, renderPos.y, scaledWidth, scaledHeight, YELLOW );
    // DrawCircleV(screenPos, 5.f, BLUE);  // worldPos mark
    DrawText( TextFormat("%01.01f",worldPos.x), renderPos.x + 5, renderPos.y + scaledHeight - 20, 10, WHITE );
    DrawText( TextFormat("%01.01f",worldPos.y), renderPos.x + 5, renderPos.y + scaledHeight - 10, 10, WHITE );
}

void Item::render(){
    Vector2 renderPos{getRenderPos()};

    // draw item
    Rectangle source{frameWidth*frame, 0.f, frameWidth, frameHeight};
    Rectangle dest{renderPos.x, renderPos.y, frameWidth * scale, frameHeight * scale};
    DrawTexturePro(*texture, source, dest, Vector2{0.f,0.f}, 0.f, WHITE);

    // DrawText(TextFormat("%02.02f",moveTimer), screenPos.x, screenPos.y, 10, WHITE);
}