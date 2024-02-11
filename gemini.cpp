
#include <raylib.h>
#include <string>
#include <vector>
#include "fuzzywuzzy.h" // Include the fuzzywuzzy library

const int MAX_SEARCH_RESULTS = 10;
const int INPUT_BOX_WIDTH = 200;
const int INPUT_BOX_HEIGHT = 30;
const int RESULT_BOX_WIDTH = INPUT_BOX_WIDTH;
const int RESULT_BOX_HEIGHT = 30;
const int RESULT_FONT_SIZE = 16;

Font defaultFont = GetFontDefault();

struct SearchResult {
    std::string text;
    int score; // FuzzyWuzzy score for better sorting
};

std::vector<SearchResult> searchResults;

void clearSearchResults() {
    searchResults.clear();
}

void addSearchResult(const std::string& text, int score) {
    if (searchResults.size() >= MAX_SEARCH_RESULTS) {
        searchResults.pop_back(); // Maintain maximum results
    }
    searchResults.insert(searchResults.begin(), {text, score});
}

void updateSearchResults(const std::string& searchTerm) {
    clearSearchResults();

    // Replace with your actual data source
    static const std::vector<std::string> data = {
        "Apple", "Banana", "Cherry", "Date", "Elderberry", "Fig", "Grape", "Honeydew", "Mango", "Nectarine",
        "Orange", "Papaya", "Peach", "Pear", "Pomegranate", "Raspberry", "Strawberry", "Tangerine", "Watermelon",
    };

    for (const std::string& item : data) {
        int score = fuzz::partial_ratio(searchTerm, item);
        if (score > 0) {
            addSearchResult(item, score);
        }
    }
}

void drawSearchBar(Rectangle inputBox, Rectangle resultBox) {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    // Input box
    DrawRectangleLinesEx(inputBox, 2, BLACK);
    if (IsMouseHoveringRectangle(inputBox)) {
        DrawRectangleLinesEx(inputBox, 3, LIGHTGRAY);
    }

    // Input text
    std::string searchTerm = GetTextEntry();
    DrawText(searchTerm.c_str(), inputBox.x + 5, inputBox.y + 5, 20, BLACK);

    // Result box
    if (!searchTerm.empty()) {
        updateSearchResults(searchTerm);
        DrawRectangleLinesEx(resultBox, 2, BLACK);
        for (int i = 0; i < std::min((int)searchResults.size(), MAX_SEARCH_RESULTS); i++) {
            const SearchResult& result = searchResults[i];
            DrawText(result.text.c_str(), resultBox.x + 5, resultBox.y + 5 + i * RESULT_FONT_SIZE, RESULT_FONT_SIZE,
                     ColorFromScore(result.score));
        }
    }

    EndDrawing();
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Fuzzy Search Bar");

    Rectangle inputBox = {50, 50, INPUT_BOX_WIDTH, INPUT_BOX_HEIGHT};
    Rectangle resultBox = {inputBox.x, inputBox.y + inputBox.height + 5, RESULT_BOX_WIDTH, RESULT_BOX_HEIGHT * MAX_SEARCH_RESULTS};

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

        drawSearchBar(inputBox, resultBox);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
