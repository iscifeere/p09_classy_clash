#include "raylib.h"
#include "Tex.h"

class Prop
{
public:
    Prop(Vector2 pos);
    Prop(Vector2 pos, Texture2D* texture_ptr);
    void Render(Vector2 knightPos);
    Rectangle getCollisionRec(Vector2 knightPos);
private:
    Texture2D* texture{nullptr};
    Vector2 worldPos{};
    float scale{8.f};
};