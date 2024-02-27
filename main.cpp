#include "./different_logic_gate.cpp"
int main() {
  {
    const u32 ScreenWidth = 800;
    const u32 ScreenHeight = 600;
    InitWindow(ScreenWidth, ScreenHeight, "Logic gate simulator");
  }

  SetTargetFPS(60);
  NotGate n1 = NotGate({20, 50});
  AndGate a1 = AndGate({50, 60});
  OrGate o1 = OrGate({10, 30});
 ObjectSet<object> obj_group{&a1,&o1,&n1,&Spline::splines};
  //obj_group.add(&a1);
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
