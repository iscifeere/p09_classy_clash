#ifndef ENTITY_H
#define ENTITY_H

#include "raylib.h"
#include "Tex.h"

class Entity
{
public:
    Entity(){};

    virtual void render() = 0;

    virtual Vector2 getScreenPos() = 0;
    Texture2D* getTexture() { return texture; }
    float getHeight() { return frameHeight*scale; }

protected:
    Texture2D* texture;

    float frameWidth{};       // width of one animation frame
    float frameHeight{};      // height of one animation frame
    float scale{};            // texture scaling
};

#endif