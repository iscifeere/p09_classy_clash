#include "Prop.h"
#include "raymath.h"

Prop::Prop(Vector2 pos, Character* player_ptr) :
    worldPos(pos),
    player(player_ptr),
    data(&ROCK_PROPDATA)
{
    
}

Prop::Prop(Vector2 pos, const propData* prop_data, Character* player_ptr) :
    worldPos(pos),
    data(prop_data),
    texture(prop_data->texture),
    player(player_ptr)
{
    frameWidth = texture->width;
    frameHeight = texture->height;
    scale = 8.f;
}

void Prop::render()
{
    Vector2 screenPos{getScreenPos()};
    if(texture != nullptr) DrawTextureEx(*texture, screenPos, 0.f, scale, WHITE);
    else DrawTextureEx(Tex::texture_prop_rock, screenPos, 0.f, scale, RED);
}

Rectangle Prop::getCollisionRec()
{
    Vector2 screenPos{getScreenPos()};
    float scaledWidth = frameWidth*scale;
    float scaledHeight = frameHeight*scale;

    return Rectangle{
        // displacement
        screenPos.x + scaledWidth * data->collisionBox.x,
        screenPos.y + scaledHeight * data->collisionBox.y,

        // scaling
        scaledWidth * data->collisionBox.width,
        scaledHeight * data->collisionBox.height
    };
}

Vector2 Prop::getScreenPos(){
    return Vector2Subtract(worldPos, player->getWorldPos());
}

void Prop::showDebugData(){
    Vector2 screenPos{getScreenPos()};
    Rectangle collisionRec{getCollisionRec()};
    DrawRectangleLines( screenPos.x, screenPos.y, frameWidth*scale, frameHeight*scale, RED );   // sprite box
    DrawRectangleLines( collisionRec.x, collisionRec.y, collisionRec.width, collisionRec.height, YELLOW );  // collision box
    DrawText( TextFormat("%01.01f",worldPos.x), collisionRec.x + 5, collisionRec.y + collisionRec.height - 20, 10, WHITE );
    DrawText( TextFormat("%01.01f",worldPos.y), collisionRec.x + 5, collisionRec.y + collisionRec.height - 10, 10, WHITE );
}