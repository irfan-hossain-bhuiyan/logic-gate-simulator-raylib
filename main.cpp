#include "./different_logic_gate.cpp"
int main() {
  {
    const u32 ScreenWidth = 800;
    const u32 ScreenHeight = 600;
    InitWindow(ScreenWidth, ScreenHeight, "Logic gate simulator");
  }

  SetTargetFPS(60);
  NotGate a1 = NotGate({20, 50});
  NotGate a2 = NotGate({50, 60});
  NotGate n1 = NotGate({10, 30});
 ObjectSet<object> obj_group{&a1,&a2,&n1,&Spline::splines};
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
