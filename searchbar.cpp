
#include "raylib.h"
#include <string>
#include <vector>

class SearchBar {
private:
    Rectangle bounds;
    std::string searchString;
    std::vector<std::string> searchResults;
    bool enabled;

public:
    SearchBar(float x, float y, float width) : bounds{ x, y, width, 30 }, enabled(false) {}

    void update() {
        // Toggle search bar with Ctrl+S
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) {
            enabled = !enabled;
        }

        // Update search string and results
        if (enabled) {
            UpdateSearch();
        }
    }

    void draw() {
        // Draw search bar outline
        DrawRectangleLinesEx(bounds, 1, DARKGRAY);

        if (enabled) {
            // Draw search string
            DrawText(searchString.c_str(), bounds.x + 5, bounds.y + 5, 20, BLACK);
        }
    }

    void UpdateSearch() {
        // Perform fuzzy search here (not implemented)
        // For demonstration, just echoing the search string as result
        searchResults.clear();
        searchResults.push_back(searchString);
    }

    bool isMouseOutsideSearchBar() {
        return !CheckCollisionPointRec(GetMousePosition(), bounds);
    }

    void handleInput() {
        // Toggle search bar when clicked outside
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && isMouseOutsideSearchBar()) {
            enabled = false;
        }

        // Input handling only when search bar is enabled
        if (enabled) {
            // Handle typing for search
            int key = GetKeyPressed();
            if (key > 0 && key != KEY_ENTER && key != KEY_ESCAPE) {
                searchString += (char)key;
            }

            // Handle backspace
            if (IsKeyPressed(KEY_BACKSPACE) && searchString.length() > 0) {
                searchString.pop_back();
            }
        }
    }
};

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Search Bar Example");

    SearchBar searchBar(10, 10, 200);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        searchBar.update();
        searchBar.draw();
        searchBar.handleInput();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
