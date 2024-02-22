#include "./basic_template.cpp"
#include "raygui.h"
#include "raylib.h"
#include <iostream>
#include <string>
#include <vector>
// Function to filter words based on input text
std::vector<std::string> filterWords(const std::string &inputText,
                                     const std::vector<std::string> &words) {
  std::vector<std::string> filteredWords;
  for (const std::string &word : words) {
    if (word.find(inputText) != std::string::npos) {
      filteredWords.push_back(word);
    }
  }
  return filteredWords;
}

int main() {
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "Raygui Searchbar Example");

  // List of words
  std::vector<std::string> words = {"apple",    "banana", "cat",   "dog",
                                    "elephant", "fish",   "grape", "horse",
                                    "iguana",   "jaguar", "kiwi"};

  char searchText[128] = {0}; // Text buffer for the search bar
  int selectedWord = -1;      // Index of selected word in dropdown menu
 SetTargetFPS(60);

  while (!WindowShouldClose()) {
    // Update filtered words based on search text
    std::vector<std::string> filteredWords = filterWords(searchText, words);
    std::string filtered_word = vectorToString(filteredWords);
    BeginDrawing();
    ClearBackground(RAYWHITE);
    // Draw search bar
    GuiTextBox(Rectangle{10, 10, 100, 30}, searchText, 128, true);
    // Draw dropdown menu with filtered words
    if (GuiDropdownBox(Rectangle{10, 40, 100, 30}, filtered_word.c_str(),
                       &selectedWord, true)) {
      // Dropdown menu selection changed, perform action here
      if (selectedWord >= 0 && selectedWord < filteredWords.size()) {
        // Print selected word to console
        std::cout << "Selected word: " << filteredWords[selectedWord] << "\n";
      }
    }

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
