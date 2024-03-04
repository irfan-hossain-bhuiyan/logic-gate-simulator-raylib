
#include "raylib.h"

// Function to draw an AND gate using Bezier curves
void DrawANDGate(int posX, int posY, bool input1, bool input2) {
  // Draw inputs
  DrawCircle(posX - 60, posY - 20, 10, input1 ? RED : BLACK);
  DrawCircle(posX - 60, posY + 20, 10, input2 ? RED : BLACK);

  // Draw AND gate
  Vector2 points[8] = {{(float)posX - 40, (float)posY - 30},
                       {(float)posX + 20, (float)posY - 30},
                       {(float)posX + 40, (float)posY - 15},
                       {(float)posX + 40, (float)posY + 15},
                       {(float)posX + 20, (float)posY + 30},
                       {(float)posX - 40, (float)posY + 30},
                       {(float)posX - 40, (float)posY - 30},
                       {(float)posX - 40, (float)posY}};
  for (auto i = 0; i < 7; i++) {
    DrawLineBezier(points[i], points[i + 1], 8, BLACK);
  }
  DrawCircle(posX + 50, posY, 10, BLACK);
}

// Function to draw an OR gate using Bezier curves
void DrawORGate(int posX, int posY, bool input1, bool input2) {
  // Draw inputs
  DrawCircle(posX - 60, posY - 20, 10, input1 ? RED : BLACK);
  DrawCircle(posX - 60, posY + 20, 10, input2 ? RED : BLACK);

  // Draw OR gate
  Vector2 points[8] = {{(float)posX - 40, (float)posY - 30},
                       {(float)posX + 20, (float)posY - 30},
                       {(float)posX + 40, (float)posY - 15},
                       {(float)posX + 40, (float)posY + 15},
                       {(float)posX + 20, (float)posY + 30},
                       {(float)posX - 40, (float)posY + 30},
                       {(float)posX - 40, (float)posY - 30},
                       {(float)posX + 50, (float)posY}};
  for (auto i = 0; i < 7; i++) {
    DrawLineBezier(points[i], points[i + 1], 8, BLACK);
  }
  DrawCircle(posX + 60, posY, 10, BLACK);
}

int main() {
  // Initialization
  const int screenWidth = 800;
  const int screenHeight = 450;
  InitWindow(screenWidth, screenHeight,
             "Raylib AND and OR Gates with Bezier Curves");

  SetTargetFPS(60);

  // Main loop
  while (!WindowShouldClose()) {
    // Draw
    BeginDrawing();
    ClearBackground(WHITE);

    // Draw AND gate
    DrawText("AND", 50, 50, 20, BLACK);
    DrawANDGate(150, 100, false, true); // Example inputs

    // Draw OR gate
    DrawText("OR", 50, 200, 20, BLACK);
    DrawORGate(150, 250, true, false); // Example inputs

    EndDrawing();
  }

  // Close window and OpenGL context
  CloseWindow();

  return 0;
}
