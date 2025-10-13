#ifndef BASE_CHARACTER_H
#define BASE_CHARACTER_H

#include "raylib.h"
#include "Tex.h"
#include "Entity.h"

class BaseCharacter : public Entity
{
public:
    BaseCharacter();
    
    // Vector2 getWorldPos(){ return worldPos; }
    Vector2 getWorldPos(){ return worldPos; }
    void undoMovementX();
    void undoMovementY();
    virtual bool tick(float deltaTime);
    // virtual Vector2 getScreenPos() = 0;
    bool getAlive() { return alive; }
    void setAlive( bool isAlive ) { alive = isAlive; }
    void setSpeed( float newSpeed ) { speed = newSpeed; }
    virtual void takeDamage(float damage);
    float getHealth(){ return health; }
    void setDrawColor(Color newColor){ drawColor = newColor; }
    bool getInvul(){ return invul; }
    virtual void deathSequence() = 0;
    virtual void addHealth( float healthAdd ) { health += healthAdd; }
    Vector2& getVelocity(){ return velocity; }      // returns a reference
    virtual void render() override;
    virtual void showDebugData() = 0;
    void setWorldPos( Vector2 newPos ){ worldPos = newPos; }
    void checkMapBoundsCollision();
    
protected:
    Texture2D* texture{&Tex::texture_knight_idle};
    Texture2D* idle{&Tex::texture_knight_idle};
    Texture2D* run{&Tex::texture_knight_run};
    Vector2 worldPos{};
    Vector2 worldPosLastFrame{};        // unused
    // 1 : facing right | -1 : facing left
    float rightLeft{1.f};

    // animation variables
    float runningTime{};
    int frame{};
    int frameRow{};
    int maxFrames{6};
    int maxFramesIdle{6};
    int maxFramesRun{6};
    int maxFrameRows{1};
    bool ignoreFrameRows{false};
    float updateTime{1.f/12.f};
    float speed{5.f};
    // float frameWidth{};
    // float frameHeight{};
    // float scale{8.0f};
    
    Vector2 velocity{};
    Vector2 movement{};
    float health{100.f};
    Color drawColor{WHITE};
    float hurtTime{};
    bool invul{false};  // invulnerable
private:
    bool alive{true};
};

#endif