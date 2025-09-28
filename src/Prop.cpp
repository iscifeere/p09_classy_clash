#include "Prop.h"
#include "raymath.h"

Prop::Prop(Vector2 pos, Character* player_ptr) :
    worldPos(pos),
    player(player_ptr)
{
    texture = &Tex::texture_prop_rock;      // default texture
}

Prop::Prop(Vector2 pos, Texture2D* texture_ptr, Character* player_ptr) :
    worldPos(pos),
    texture(texture_ptr),
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
    return Rectangle{
        screenPos.x,
        screenPos.y,
        texture->width * scale,
        texture->height * scale
    };
}

Vector2 Prop::getScreenPos(){
    return Vector2Subtract(worldPos, player->getWorldPos());
}

void Prop::showDebugData(){
    Vector2 screenPos{getScreenPos()};
    DrawRectangleLines( screenPos.x, screenPos.y, frameWidth*scale, frameHeight*scale, YELLOW );
    DrawText( TextFormat("%01.01f",worldPos.x), screenPos.x, screenPos.y + frameHeight*scale - 20, 10, WHITE );
    DrawText( TextFormat("%01.01f",worldPos.y), screenPos.x, screenPos.y + frameHeight*scale - 10, 10, WHITE );
}