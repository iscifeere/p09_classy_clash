#include "Item.h"
#include "raymath.h"
#include <cmath>
#include "EntityManager.h"
#include <iostream>

void Item::init(){
    std::cout << "[Item init function (" << this << ") ]" << std::endl;

    scale = 4.f;

    texture = data->texture;
    maxFrames = data->maxFrames;
    updateTime = data->updateTime;
    item_effect = data->effect;

    frameWidth = texture->width / maxFrames;
    frameHeight = texture->height;
}

Item::Item(){
    std::cout << "\n[Item default constructor (" << this << ") ]" << std::endl;
    setAlive(false);
    player = EntityMng::getPlayerPtr();
    data = &HEART_ITEMDATA;
    init();
}

Item::Item(Vector2 pos) :
    worldPos(pos)
{
    player = EntityMng::getPlayerPtr();
    data = &HEART_ITEMDATA;
    init();
}

Item::Item(Vector2 pos, const itemData* item_data ) :
    worldPos(pos),
    data(item_data)
{
    player = EntityMng::getPlayerPtr();
    init();
}

void Item::spawnReset(Vector2 pos, const itemData* item_data )
{
    worldPos = pos;
    data = item_data;
    init();
    setAlive(true);
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