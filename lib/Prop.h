#ifndef PROP_H
#define PROP_H

#include "raylib.h"
#include "Tex.h"
#include "Entity.h"
#include "Character.h"

struct propData{
    Texture2D* texture;
    Rectangle collisionBox;     // proportions in relation to texture
};

class Prop : public Entity
{
public:
    Prop(Vector2 pos, Character* player_ptr);
    Prop(Vector2 pos, const propData* prop_data, Character* player_ptr);
    void render() override;
    Rectangle getCollisionRec();
    Rectangle getCollisionRecOld();
    Vector2 getScreenPos() override;
    void showDebugData();

private:
    Texture2D* texture{&Tex::texture_prop_rock};    // default texture
    Vector2 worldPos{};
    Character* player{nullptr};
    const propData* data{nullptr};
};

//==============================

const propData ROCK_PROPDATA{
    .texture = &Tex::texture_prop_rock,
    .collisionBox = Rectangle{
        .x = 0,
        .y = 0.25f,
        .width = 1,
        .height = 0.75f
    }
};
const propData LOG_PROPDATA{
    .texture = &Tex::texture_prop_log,
    .collisionBox = Rectangle{
        .x = 0.25f,
        .y = 0.25f,
        .width = 0.5f,
        .height = 0.5f
    }
};
const propData SIGN_PROPDATA{
    .texture = &Tex::texture_prop_sign,
    .collisionBox = Rectangle{
        .x = 0.4f,
        .y = 0.66f,
        .width = 0.2f,
        .height = 0.25f
    }
};

#endif // PROP_H