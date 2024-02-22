
#include "raygui.h"
#include "raylib.h"

int main() {
  const int screenWidth = 800;
  const int screenHeight = 450;
  Vector2 scroll;
  InitWindow(screenWidth, screenHeight, "Raygui Containers Example");
  Rectangle view_rec;
  // Define a rectangle for the panel
  Rectangle panelRec = {100, 100, 200, 200};

  // Define a rectangle for the scroll panel
  Rectangle scrollPanelRec = {400, 100, 200, 200};

  // Define a rectangle for the group box
  Rectangle groupBoxRec = {100, 350, 200, 100};

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Draw a panel with a border and title
    GuiPanel(panelRec, "Panel");

    // Draw a scroll panel with a border and title
    GuiScrollPanel(scrollPanelRec,
                   "working",
                   (Rectangle){10, 10, 100, 100}, &scroll, &view_rec);
    // Draw a group box with a border and title
    GuiGroupBox(groupBoxRec, "GroupBox");

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
