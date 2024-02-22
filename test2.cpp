
#include <raylib.h>
#include "raygui.h"

int main() {
    InitWindow(800, 600, "Search Bar Example");
    Font defaultFont = GetFontDefault();

    // Search bar variables
    const int searchBarWidth = 200;
    const int searchBarHeight = 30;
    const int searchBarPosX = 200;
    const int searchBarPosY = 100;
    bool searchFocused = false;
    char searchText[128] = "";

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw search bar background
        DrawRectangle(searchBarPosX, searchBarPosY, searchBarWidth, searchBarHeight, LIGHTGRAY);

        // Draw search icon
        if (!searchFocused) {
            DrawTextEx(defaultFont, "", Vector2{searchBarPosX + 5, searchBarPosY + 6},
                      defaultFont.baseSize, 1.0f, DARKGRAY);
        }

        // Input field for search text
        searchFocused = GuiTextBox(
            Rect{searchBarPosX + 20, searchBarPosY + 2, searchBarWidth - 40, searchBarHeight - 4},
            searchText, sizeof(searchText), searchFocused);

        // Search icon animation when focused
        if (searchFocused) {
            float blinkTime = GetTimeDiff(0) / 0.5f;
            Color iconColor = (int)(blinkTime % 2) ? BLACK : GRAY;
            DrawTextEx(defaultFont, "", Vector2{searchBarPosX + 5, searchBarPosY + 6},
                      defaultFont.baseSize, 1.0f, iconColor);
        }

        // Display search text below the bar
        if (strlen(searchText) > 0) {
            DrawTextEx(defaultFont, searchText, Vector2{searchBarPosX + 20, searchBarPosY + searchBarHeight},
                      defaultFont.baseSize, 1.0f, BLACK);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
