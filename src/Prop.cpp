#include "Prop.h"
#include "raymath.h"
#include "EntityManager.h"

Prop::Prop()
{
    setAlive(false);
    player = EntityMng::getPlayerPtr();
}

Prop::Prop(Vector2 pos) :
    worldPos(pos),
    player(EntityMng::getPlayerPtr()),
    data(&ROCK_PROPDATA)
{
    
}

Prop::Prop(Vector2 pos, const propData* prop_data) :
    worldPos(pos),
    data(prop_data),
    texture(prop_data->texture),
    player(EntityMng::getPlayerPtr())
{
    frameWidth = texture->width;
    frameHeight = texture->height;
    scale = 8.f;
}

void Prop::spawnReset(Vector2 pos, const propData* prop_data){
    worldPos = pos;
    data = prop_data;
    texture = prop_data->texture;

    frameWidth = texture->width;
    frameHeight = texture->height;
    scale = 8.f;
    
    setAlive(true);
}

void Prop::render()
{
    Vector2 renderPos{getRenderPos()};

    if(texture != nullptr) DrawTextureEx(*texture, renderPos, 0.f, scale, WHITE);
    else DrawTextureEx(Tex::texture_prop_rock, renderPos, 0.f, scale, RED);
}

Rectangle Prop::getCollisionRec()
{
    Vector2 renderPos{ getRenderPos() };
    float scaledWidth = frameWidth*scale;
    float scaledHeight = frameHeight*scale;

    return Rectangle{
        // displacement
        renderPos.x + ( scaledWidth * data->collisionBox.x ),
        renderPos.y + ( scaledHeight * data->collisionBox.y ),

        // scaling
        scaledWidth * data->collisionBox.width,
        scaledHeight * data->collisionBox.height
    };
}

Rectangle Prop::getCollisionRecWorPos()
{
    float scaledWidth = frameWidth*scale;
    float scaledHeight = frameHeight*scale;
    Vector2 worldPosTopLeft{
        worldPos.x - scaledWidth*0.5f,
        worldPos.y - scaledHeight*0.5f
    };

    return Rectangle{
        // displacement
        worldPosTopLeft.x + ( scaledWidth * data->collisionBox.x ),
        worldPosTopLeft.y + ( scaledHeight * data->collisionBox.y ),

        // scaling
        scaledWidth * data->collisionBox.width,
        scaledHeight * data->collisionBox.height
    };
}

Vector2 Prop::getScreenPos(){
    return Vector2Subtract(worldPos, player->getWindowOriginWorPos());
}

void Prop::showDebugData(){
    Vector2 screenPos{getScreenPos()};
    Vector2 renderPos{getRenderPos( screenPos )};
    Rectangle collisionRec{getCollisionRec()};

    DrawRectangleLines( renderPos.x, renderPos.y, frameWidth*scale, frameHeight*scale, RED );   // sprite box
    DrawRectangleLines( collisionRec.x, collisionRec.y, collisionRec.width, collisionRec.height, YELLOW );  // collision box
    DrawCircleV(screenPos, 5.f, BLUE);  // worldPos mark
    DrawText( TextFormat("%01.01f",worldPos.x), collisionRec.x + 5, collisionRec.y + collisionRec.height - 20, 10, WHITE );
    DrawText( TextFormat("%01.01f",worldPos.y), collisionRec.x + 5, collisionRec.y + collisionRec.height - 10, 10, WHITE );
}