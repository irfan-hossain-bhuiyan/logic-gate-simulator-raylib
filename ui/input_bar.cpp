#include "../object.cpp"
#include <iostream>
#include "raylib.h"
#include <string>
#include <vector>
class input_bar:public Node2d,clickable{
	private:
		int cursorPos;
		bool isEditing=false;
		std::string searchtext="";

};
// Function to perform fuzzy search
bool fuzzyMatch(const std::string& pattern, const std::string& text) {
    size_t patternIdx = 0;
    for (size_t i = 0; i < text.size() && patternIdx < pattern.size(); ++i) {
        if (tolower(text[i]) == tolower(pattern[patternIdx])) {
            ++patternIdx;
        }
    }
    return patternIdx == pattern.size();
}
int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Fuzzy Search Bar with Cursor - raylib");

    std::vector<std::string> items = {"apple", "banana", "grape", "orange", "pear", "pineapple"};
    std::vector<std::string> filteredItems;

    std::string searchText = "";
    int cursorPos = 0;
    bool isEditing = false;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // Draw search bar
        DrawRectangle(10, 10, screenWidth - 20, 30, LIGHTGRAY);
        if (isEditing) {
            DrawRectangleLines(10 + MeasureText(searchText.substr(0, cursorPos).c_str(), 20), 10, 1, 30, BLACK); // Cursor
        }
        DrawText(searchText.c_str(), 20, 20, 20, DARKGRAY);

        // Filter items based on search text
        filteredItems.clear();
        for (const auto& item : items) {
            if (fuzzyMatch(searchText, item)) {
                filteredItems.push_back(item);
            }
        }

        // Draw filtered items
        for (size_t i = 0; i < filteredItems.size(); ++i) {
            DrawText(filteredItems[i].c_str(), 20, 70 + i * 30, 20, BLACK);
        }

        EndDrawing();

        // Input handling
        if (IsKeyPressed(KEY_BACKSPACE) && searchText.size() > 0 && isEditing) {
            searchText.erase(searchText.begin() + cursorPos - 1);
            cursorPos--;
        } else if (IsKeyPressed(KEY_ENTER)) {
            // Handle search action here, for example, you can print the search results
            for (const auto& item : filteredItems) {
                std::cout << item << std::endl;
            }
        } else {
            int key = GetKeyPressed();
            if (key > 0 && key < 256) {
                searchText.insert(cursorPos, 1, static_cast<char>(key));
                cursorPos++;
            }
        }

        // Move cursor left and right
        if (IsKeyPressed(KEY_LEFT) && cursorPos > 0) {
            cursorPos--;
        } else if (IsKeyPressed(KEY_RIGHT) && cursorPos < searchText.size()) {
            cursorPos++;
        }

        // Toggle editing mode
        if (IsKeyPressed(KEY_ENTER)) {
            isEditing = !isEditing;
        }
    }

    CloseWindow();

    return 0;
}
