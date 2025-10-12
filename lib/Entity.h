#ifndef ENTITY_H
#define ENTITY_H

#include "raylib.h"
#include "Tex.h"

class Entity
{
public:
    Entity(){};

    virtual bool tick(float deltaTime) = 0;
    virtual void render() = 0;

    bool getAlive() { return alive; }
    void setAlive( bool isAlive ) { alive = isAlive; }
    virtual Vector2 getScreenPos() = 0;
    Texture2D* getTexture() { return texture; }
    
    Vector2 getRenderPos(){     // adjusted rendering position so that screenPos is at the centre
        Vector2 screenPos{getScreenPos()};
        return Vector2{screenPos.x - getWidth()*0.5f, screenPos.y - getHeight()*0.5f};
    }
    Vector2 getRenderPos(Vector2 screenPos){
        return Vector2{screenPos.x - getWidth()*0.5f, screenPos.y - getHeight()*0.5f};
    }

    float getWidth() { return frameWidth*scale; }   // returns scaled width
    float getHeight() { return frameHeight*scale; } // returns scaled height

protected:
    Texture2D* texture;

    float frameWidth{};       // width of one animation frame
    float frameHeight{};      // height of one animation frame
    float scale{};            // texture scaling

    bool alive{true};
};

#endif