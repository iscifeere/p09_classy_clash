#ifndef GENENTITY_H
#define GENENTITY_H

#include "raylib.h"
#include "Tex.h"
#include "Character.h"
#include "Entity.h"

class GenEntity : public Entity
{
public:
    GenEntity();
    GenEntity( Vector2 pos, Character* player );
    GenEntity( Vector2 pos, Vector2 direction, Character* player );

    void spawnReset();
    void spawnReset(Vector2 pos, Vector2 direction, Character* playerPtr);

    bool tick(float deltaTime) override;
    Vector2 getWorldPos() { return worldPos; }
    Vector2 getScreenPos() override;
    Rectangle getCollisionRec();
    // bool getAlive() { return alive; }
    // void setAlive( bool isAlive ) { alive = isAlive; }
    void showDebugData();
    void render() override;

    Vector2 worldPos{};
    Character* player{nullptr};
    Texture2D* texture{&Tex::texture_heart};
    Vector2 velocity{};
    Vector2 movement{};
    Vector2 screenPos{};
    float speed{12.f};
    int frame{};
    int maxFrames{1};
    // float frameWidth{};       // width of one animation frame
    // float frameHeight{};      // height of one animation frame
    float runningTime{};
    float updateTime{1.f/12.f};     // speed of animation
    // float scale{4.f};
    Color drawColor{BLUE};
    float moveTimer{0.f};
    // bool active{true};
    
private:
    // bool alive{true};
};

#endif
