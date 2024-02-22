#include "./searchbar.cpp"
#include "object.cpp"
#include <vector>
int main() {
  {
    const u32 ScreenWidth = 800;
    const u32 ScreenHeight = 600;
    InitWindow(ScreenWidth, ScreenHeight, "Ui check");
  }

  SetTargetFPS(60);
  ObjectSet<SearchBar> obj_group;
  SearchBar sb{Vector2{10,10}};
  sb.texts=std::vector<std::string>{"And","Not","Or"};
  obj_group.add(&sb);
  // obj_group.add(&a2);
  //obj_group.add(&n1);
 // obj_group.add(&Spline::splines);
  obj_group.ready();
  while (!WindowShouldClose()) {
    manager::onready(); // For a data to the frame number it is currently on.
    obj_group.update();
    BeginDrawing();
    ClearBackground(RAYWHITE);
    obj_group.draw();
    manager::lastdrawing();
    EndDrawing();
  }
}
