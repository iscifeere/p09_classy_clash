#ifndef PROP_H
#define PROP_H

#include "raylib.h"
#include "Tex.h"
#include "Entity.h"
#include "Character.h"

struct propData{
    Texture2D* texture;
    Rectangle collisionBox;     // Location and size of collision box.
                                // x and y are propotional displacement from sprite screen position,
                                // width and height are scaling proportional to texture.
};

class Prop : public Entity
{
public:
    Prop();
    Prop(Vector2 pos, Character* player_ptr);
    Prop(Vector2 pos, const propData* prop_data, Character* player_ptr);

    void spawnReset(Vector2 pos, const propData* prop_data, Character* player_ptr);

    bool tick(float deltaTime) {
        if(!getAlive()) return false;
        else return true;
    }
    void render();
    Rectangle getCollisionRec();            // relative position version
    Rectangle getCollisionRecWorPos();      // absolute position version
    Vector2 getWorldPos(){ return worldPos; }
    Vector2 getScreenPos() override;
    void showDebugData();
    bool getAlive(){ return alive; }
    void setAlive(bool isAlive){ alive = isAlive; }

private:
    Texture2D* texture{&Tex::texture_prop_rock};    // default texture
    Vector2 worldPos{};
    Character* player{nullptr};
    const propData* data{nullptr};
    bool alive{true};
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