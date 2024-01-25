
#include "raylib.h"

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Raylib Polygon Drawing Example");

    SetTargetFPS(60);

    Vector2 points[] = {
        (Vector2){ screenWidth / 2, screenHeight / 2 - 100 },
        (Vector2){ screenWidth / 2 + 100, screenHeight / 2 },
        (Vector2){ screenWidth / 2 - 100, screenHeight / 2 },
    };

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // Draw Polygon
        DrawPoly(points, 3, ORANGE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
