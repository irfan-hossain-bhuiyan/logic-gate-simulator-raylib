#include "./different_logic_gate.cpp"
#include "./ui/hover_bar.cpp"
#include <raylib.h>
ObjectSet<gate> gate_group;
ObjectSet<object> ui_group;
ObjectSet<object> extra_group;
void group_update() {
  gate_group.update();
  ui_group.update();
  extra_group.update();
}
void group_draw() {
  gate_group.draw();
  extra_group.draw();
  ui_group.draw();
}
void _object_click_update(){
	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		clickable::selected=nullptr;
		for(auto x:gate_group){
			x->click_update.trigger_event();
		}
		clickable::current_selected=clickable::selected;
	}
}
void input_update(){
	_object_click_update();	
}

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
  Switch s1 = Switch({100, 30});
  Light l1 = Light({100, 60});
  HoverBar hb;
  // ObjectSet<object> obj_group{&a1,&o1,&n1,&l1,&s1,&Spline::splines};
  gate_group = ObjectSet<gate>{&n1, &a1, &o1, &s1, &l1};
  extra_group = ObjectSet<object>{&Spline::splines};
  ui_group =ObjectSet<object>{&hb};
  // obj_group.add(&a1);
  //  obj_group.add(&a2);
  // obj_group.add(&n1);
  // obj_group.add(&Spline::splines);
  while (!WindowShouldClose()) {
    manager::onupdate(); // For a data to the frame number it is currently on.
    input_update();//input update should be before of group update,else there are error.
    group_update();
    BeginDrawing();
    ClearBackground(RAYWHITE);
    group_draw();
    manager::lastdrawing();
    EndDrawing();
  }
}
