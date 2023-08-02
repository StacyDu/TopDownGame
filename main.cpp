#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Prop.h"
#include "Enemy.h"
#include <string>
#include <algorithm>

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
   
    Prop props[2]{
        Prop{Vector2{600.f, 300.f}, LoadTexture("nature_tileset/Rock.png")},
        Prop{Vector2{400.f, 500.f}, LoadTexture("nature_tileset/Log.png")},
    };

    Enemy goblin{
        Vector2{900.f, 400.f}, 
        LoadTexture("characters/goblin_idle_spritesheet.png"), 
        LoadTexture("characters/goblin_run_spritesheet.png"),
        2.0f
    };

    Enemy goblin2{
    Vector2{900.f, 800.f}, 
    LoadTexture("characters/goblin_idle_spritesheet.png"), 
    LoadTexture("characters/goblin_run_spritesheet.png"),
    2.0f
    };

    Enemy slime{
        Vector2{500.f, 700.f},
        LoadTexture("characters/slime_idle_spritesheet.png"), 
        LoadTexture("characters/slime_run_spritesheet.png"),
        1.5f
    };
    Enemy* enemies[]{
        &goblin,
        &slime,
        &goblin2
    };

    for (auto enemy : enemies)
    {
        enemy->setTarget(&knight);
    }

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        mapPos = Vector2Scale(knight.getWorldPos(), -1.f);

        // draw map 
        DrawTextureEx(map, mapPos, 0.0, mapScale, WHITE);

        // draw the props
        for (auto prop : props)
        {
            prop.Render(knight.getWorldPos());
        }

        // print amount of alive enemies
        int enemiesAlive{};
        for (auto enemy : enemies)
        {
            enemy->tick(GetFrameTime());
            if (enemy->getAlive())
            {
                enemiesAlive++;
            }
        }
        std::string numberOfEnemies = "Enemies left: ";
        numberOfEnemies.append(std::to_string(enemiesAlive));
        DrawText(numberOfEnemies.c_str(), 10.f, 10.f, 20, RED); 

        if (!knight.getAlive()) // is not alive
        {
            DrawText("Game Over!", 55.f, 45.f, 40, RED);
            EndDrawing();
            continue;
        }
        else if (!enemiesAlive)
        {
            DrawText("You won!", knight.getScreenPos().x, knight.getScreenPos().y, 40, GREEN);
            EndDrawing();
            continue;
        }
        else // knight is alive
        {
            std::string knightsHealth = "Health: ";
            knightsHealth.append(std::to_string(knight.getHealth()), 0, 4);
            
            DrawText(knightsHealth.c_str(), 55.f, 45.f, 40, RED);
        }      

        knight.tick(GetFrameTime());
        // check map bouncess
        if (knight.getWorldPos().x < 0.f ||
            knight.getWorldPos().y < 0.f ||
            knight.getWorldPos().x + windowWidth > map.width * mapScale ||
            knight.getWorldPos().y + windowHeight > map.height * mapScale)
        {
            knight.undoMovement();
        }
        // check collision
        for (auto prop : props)
        {
            if (CheckCollisionRecs(knight.getCollisionRec(), prop.getCollisionRec(knight.getWorldPos())))
            {
                knight.undoMovement();
            };
        }       
  
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            for (auto enemy : enemies)
            {
                if (CheckCollisionRecs(enemy->getCollisionRec(), knight.getWeaponCollision()))
                {
                    enemy->setAlive(false);
                } 
            }
        }

        EndDrawing();
    }
    UnloadTexture(map);
    CloseWindow();
}
