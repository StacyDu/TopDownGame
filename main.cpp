#include "raylib.h"
#include "raymath.h"
#include "Character.h"


int main () {

    // init window
    const int windowWidth{384};
    const int windowHeight{384};
    InitWindow(windowWidth, windowHeight, "My first Top Down Game");

    // load map
    Texture2D map = LoadTexture("nature_tileset/map.png");
    Vector2 mapPos{0.0, 0.0};
    const float mapScale{4.0f};

    Character knight(windowWidth, windowHeight);

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        mapPos = Vector2Scale(knight.getWorldPos(), -1.f);

        // draw map 
        DrawTextureEx(map, mapPos, 0.0, mapScale, WHITE);
        knight.tick(GetFrameTime());
        // check map bounce
        if (knight.getWorldPos().x < 0.f ||
            knight.getWorldPos().y < 0.f ||
            knight.getWorldPos().x + windowWidth > map.width * mapScale ||
            knight.getWorldPos().y + windowHeight > map.height * mapScale)
        {
            knight.undoMovement();
        }

        EndDrawing();
    }
    UnloadTexture(map);
    CloseWindow();
}
