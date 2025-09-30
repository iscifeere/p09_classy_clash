#ifndef CHARACTER_H
#define CHARACTER_H

#include "raylib.h"
#include "BaseCharacter.h"

class Character : public BaseCharacter
{
public:
    Character(Vector2 pos);

    bool tick(float deltaTime) override;
    // Vector2 getWorldPos() override;
    Vector2 getScreenPos() override;
    Rectangle getWeaponCollisionRec(){ return weaponCollisionRec; }
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
};

#endif