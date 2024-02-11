
#include "raylib.h"
#include <string>
#include <vector>
#include <algorithm>

// Function to perform fuzzy search
bool fuzzyMatch(const std::string& pattern, const std::string& text) {
    size_t patternIndex = 0;
    for (size_t i = 0; i < text.size(); ++i) {
        if (tolower(pattern[patternIndex]) == tolower(text[i])) {
            ++patternIndex;
            if (patternIndex == pattern.size()) {
                return true;
            }
        }
    }
    return false;
}

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Raylib Fuzzy Search");

    std::string searchString;
    std::vector<std::string> items = {"Apple", "Banana", "Cherry", "Orange", "Peach", "Grape"};

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Input box
        DrawRectangle(10, 10, 200, 30, LIGHTGRAY);
        DrawRectangleLines(10, 10, 200, 30, DARKGRAY);
        DrawText(searchString.c_str(), 20, 20, 20, DARKGRAY);

        // Fuzzy search and display results
        int posY = 50;
        for (const auto& item : items) {
            if (fuzzyMatch(searchString, item)) {
                DrawText(item.c_str(), 10, posY, 20, BLACK);
                posY += 30;
            }
        }

        // Input handling
        if (IsKeyPressed(KEY_BACKSPACE) && searchString.size() > 0) {
            searchString.pop_back();
        } else if (IsKeyPressed(KEY_ENTER)) {
            // Perform action on Enter key press
        } else {
            // Handle text input
            int key = GetKeyPressed();
            if ((key >= 32) && (key <= 125) && (searchString.size() < 20)) {
                searchString.push_back((char)key);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
