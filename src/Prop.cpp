#include "Prop.h"
#include "raymath.h"

Prop::Prop(Vector2 pos) :
    worldPos(pos)
{
    texture = &Tex::texture_prop_rock;      // default texture
}

Prop::Prop(Vector2 pos, Texture2D* texture_ptr) :
    worldPos(pos),
    texture(texture_ptr)
{

}

void Prop::render(Vector2 knightPos)
{
    Vector2 screenPos{Vector2Subtract(worldPos, knightPos)};
    if(texture != nullptr) DrawTextureEx(*texture, screenPos, 0.f, scale, WHITE);
    else DrawTextureEx(Tex::texture_prop_rock, screenPos, 0.f, scale, RED);
}

Rectangle Prop::getCollisionRec(Vector2 knightPos)
{
    Vector2 screenPos{Vector2Subtract(worldPos, knightPos)};
    return Rectangle{
        screenPos.x,
        screenPos.y,
        texture->width * scale,
        texture->height * scale
    };
}