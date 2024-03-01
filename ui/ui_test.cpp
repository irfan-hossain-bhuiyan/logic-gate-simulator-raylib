#include "./hover_bar.cpp"
#include <string>
#include <vector>
ObjectSet<object> ui_group;
std::vector<std::string> mb_texts = {"Option 1", "Option 2"};
std::vector<std::string> gsb_texts={"Gate 1","Gate 2","Gate 3"};
MouseMenuBox mb(&mb_texts);
GateSearchBar gsb(&gsb_texts);
void menu_select(std::string &text) {
  Debug::add_text(text);
  gsb.enable();
}
void create_gate(std::string &text) { Debug::add_text(text); }
int main() {
  {
    const u32 ScreenWidth = 800;
    const u32 ScreenHeight = 600;
    InitWindow(ScreenWidth, ScreenHeight, "Logic gate simulator");
  }
  mb.on_select.add_link([](auto x) { menu_select(x); });
  gsb.on_enter_press.add_link([](auto x) { create_gate(x); });
  SetTargetFPS(60);
  // ObjectSet<object> obj_group{&a1,&o1,&n1,&l1,&s1,&Spline::splines};
  ui_group = ObjectSet<object>{&mb,&gsb};
  // obj_group.add(&a1);
  //  obj_group.add(&a2);
  // obj_group.add(&n1);
  // obj_group.add(&Spline::splines);
  while (!WindowShouldClose()) {
    manager::onupdate(); // For a data to the frame number it is currently on.
    ui_group.update();
    BeginDrawing();
    ClearBackground(RAYWHITE);
    ui_group.draw();
    manager::lastdrawing();
    EndDrawing();
  }
}
