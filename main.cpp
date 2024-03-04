#include "./different_logic_gate.cpp"
#include "./ui/hover_bar.cpp"
#include <raylib.h>
#include <string>
#include <vector>
std::vector<std::string> Mouse_menu{"Create", "Delete"};
std::vector<std::string> Gates{"And", "Not", "Or", "Switch", "Light"};

ObjectSet<gate> gate_group;
ObjectSet<object> ui_group;
ObjectSet<object> extra_group;
void create_gate(std::string& gate_str){
	if(gate_str=="And")gate_group.add(new AndGate(GetMousePosition()));
	else if(gate_str=="Not")gate_group.add(new NotGate(GetMousePosition()));
	else if(gate_str=="Or")gate_group.add(new OrGate(GetMousePosition()));
	else if(gate_str=="Switch")gate_group.add(new Switch(GetMousePosition()));
	else if(gate_str=="Light")gate_group.add(new Light(GetMousePosition()));
}
GateSearchBar hb = GateSearchBar(&Gates);
MouseMenuBox mb = MouseMenuBox(&Mouse_menu);
void menu_select(const std::string &option) {
  if (option == "Create") {
    hb.enable();
  }
  if (option == "Delete") {
    gate_group.delete_obj(clickable::previous_selection);
  }
}
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
void _object_click_update() {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    clickable::selected = nullptr;
    for (auto x : gate_group) {
      x->click_update.trigger_event();
    }
    clickable::previous_selection=clickable::current_selected;
    clickable::current_selected = clickable::selected;
  }
}
void input_update() { _object_click_update(); }

int main() {
  {
    const u32 ScreenWidth = 1200;
    const u32 ScreenHeight = 800;
    InitWindow(ScreenWidth, ScreenHeight, "Logic gate simulator");
  }
  mb.on_select.add_link([](auto x){menu_select(x);});
  hb.on_enter_press.add_link([](auto x){create_gate(x);});
  SetTargetFPS(60);
 // ObjectSet<object> obj_group{&a1,&o1,&n1,&l1,&s1,&Spline::splines};
  extra_group = ObjectSet<object>{&Spline::splines};
  ui_group = ObjectSet<object>{&hb, &mb};
  // obj_group.add(&a1);
  //  obj_group.add(&a2);
  // obj_group.add(&n1);
  // obj_group.add(&Spline::splines);
  while (!WindowShouldClose()) {
    manager::onupdate(); // For a data to the frame number it is currently on.
    input_update(); // input update should be before of group update,else there
                    // are error.
    group_update();
    BeginDrawing();
    ClearBackground(RAYWHITE);
    group_draw();
    manager::lastdrawing();
    EndDrawing();
  }
}
