#ifndef PROP_H
#define PROP_H

#include "raylib.h"
#include "Tex.h"
#include "Entity.h"
#include "Character.h"

class Prop : public Entity
{
public:
    Prop(Vector2 pos, Character* player_ptr);
    Prop(Vector2 pos, Texture2D* texture_ptr, Character* player_ptr);
    void render() override;
    Rectangle getCollisionRec();
    Vector2 getScreenPos() override;
    void showDebugData();

private:
    Texture2D* texture{nullptr};
    Vector2 worldPos{};
    Character* player{nullptr};
};

#endif // PROP_H