#ifndef CHARACTER_H
#define CHARACTER_H

#include "raylib.h"
#include "BaseCharacter.h"

class Character : public BaseCharacter
{
public:
    Character();
    Character(Vector2 pos);

    void init();    // initialize necessary variables, called in constructor

    bool tick(float deltaTime) override;
    // Vector2 getWorldPos() override;
    void setWorldPos(Vector2 pos){ worldPos = pos; }
    Vector2 getScreenPos() override;
    Vector2 getWindowOriginWorPos();
    Vector2 getPrevWorldPos();  // calculate worldPos from previous tick
    Rectangle getCollisionRec();        // relative position version
    Rectangle getCollisionRecWorPos();  // absolute position version
    Rectangle getPrevCollisionRecWorPos();
    Rectangle getWeaponCollisionRec(){ return weaponCollisionRec; }
    Rectangle getHurtRec();
    float getDamage(){ return damagePerHit; }
    void deathSequence() override;
    bool getIsAttack() { return isAttacking; }
    void addHealth( float healthAdd ) override;
    void setHealth( float newHealth ){ health = newHealth; }
    int getMoney(){ return moneyCount; }
    void addMoney(int moneyAdd){ moneyCount += moneyAdd; }
    void showDebugData() override;
    void showStats();
    void render() override;
    void resurrect();
    void resetState();
    void shootProyectile();
    
private:
    int windowWidth{};
    int windowHeight{};
    Texture2D* weapon{&Tex::texture_sword};
    Rectangle weaponCollisionRec{};
    float damagePerHit{20.f};
    bool isAttacking{false};
    bool healing{false};
    float healTime{};
    int moneyCount{};
    struct s{
        Vector2 origin{};
        Vector2 offset{};
        float rotation{};
    } swordVariables{};
    Color healthBarColor{WHITE};
    Rectangle collisionBox{
        .x = 0.2f,
        .y = 0.75f,
        .width = 0.6f,
        .height = 0.25f
    };
    Rectangle hurtBox{
        .x = 0.2f,
        .y = 0.2f,
        .width = 0.6f,
        .height = 0.8f
    };
};

#endif