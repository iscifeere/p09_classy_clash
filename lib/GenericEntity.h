#ifndef GENENTITY_H
#define GENENTITY_H

#include "raylib.h"
#include "Tex.h"
#include "Character.h"

class GenEntity
{                // FALTA HACER EL CPP
public:
    GenEntity( Vector2 pos, Character* player );
    GenEntity( Vector2 pos, Vector2 direction, Character* player );

    void reset();
    void reset( Vector2 pos, Vector2 direction );

    bool tick(float deltaTime);
    Vector2 getWorldPos() { return worldPos; }
    Vector2 getScreenPos();
    Rectangle getCollisionRec();
    bool getAlive() { return alive; }
    void setAlive( bool isAlive ) { alive = isAlive; }

    Vector2 worldPos{};
    Character* player{nullptr};
    Texture2D* texture{&Tex::texture_heart};
    Vector2 velocity{};
    Vector2 movement{};
    Vector2 screenPos{};
    float speed{12.f};
    int frame{};
    int maxFrames{1};
    float frameWidth{};       // width of one animation frame
    float frameHeight{};      // height of one animation frame
    float runningTime{};
    float updateTime{1.f/12.f};     // speed of animation
    float scale{4.f};
    Color drawColor{BLUE};
    float moveTimer{0.f};
    // bool active{true};
private:
    bool alive{true};
};

#endif
