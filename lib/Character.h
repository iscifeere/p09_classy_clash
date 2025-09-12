#ifndef CHARACTER_H
#define CHARACTER_H

#include "raylib.h"
#include "BaseCharacter.h"

class Character : public BaseCharacter
{
public:
    Character(int winWidth, int winHeight);
    Character(Vector2 pos, int winWidth, int winHeight);
    virtual bool tick(float deltaTime) override;
    // virtual Vector2 getWorldPos() override;
    virtual Vector2 getScreenPos() override;
    Rectangle getWeaponCollisionRec(){ return weaponCollisionRec; }
    float getDamage(){ return damagePerHit; }
    virtual void deathSequence() override;
    bool getIsAttack() { return isAttacking; }
    virtual void addHealth( float healthAdd ) override;
    int getMoney(){ return moneyCount; }
    void addMoney(int moneyAdd){ moneyCount += moneyAdd; }
    virtual void showDebugData() override;
    void showStats();
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
};

#endif