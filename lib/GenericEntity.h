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
    GenEntity( Vector2 pos, Vector2 direction);

    void init();    // initialize necessary variables, called in constructor
    void spawnReset(Vector2 pos, Vector2 directio, bool p_isEnemy);

    bool tick(float deltaTime);
    Vector2 getWorldPos() { return worldPos; }
    Vector2 getScreenPos() override;
    Rectangle getCollisionRec();
    bool getAlive() { return alive; }
    void setAlive( bool isAlive ) { alive = isAlive; }
    void showDebugData();
    void render();
    void checkPlayerCollision();

    bool getIsEnemy(){ return isEnemy; }
    void setIsEnemy(bool& p_isEnemy){
        isEnemy = p_isEnemy;
        drawColor = isEnemy ? RED : GREEN;
    }
    
private:
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

    bool alive{true};
    bool isEnemy{true};     // is enemy proyectile
};

#endif
