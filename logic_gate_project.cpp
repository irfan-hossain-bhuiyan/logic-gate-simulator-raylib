#include "./basic_template.cpp"
#include "./gate.cpp"
#include "./object.cpp"
#include <raylib.h>
#include <vector>
// const region
// end region
// declaration
struct AndGate;
// declaration end
//
// And Gate class
class AndGate : public gate {
  static constexpr float GateHeight = 30;
  static constexpr float GateWidth = 50;

public:
  AndGate(Vector2 pos = {0}) : Node2d(pos), gate(GateWidth, GateHeight, 2, 1) {
    draw_event.add_link([this]() { this->draw(); });
  }

  void draw() {
    DrawRectangleRec(rect(), RED);
    if (is_clicking) {
      DrawRectangleLinesEx(rect(), 3, BLACK);
    }
  }
  void evaluate() override {
    this->gateoutputpoints[0]->boolean_state =
        this->gateinputpoints[0]->boolean_state &&
        this->gateinputpoints[1]->boolean_state;

  }
  // functions
private:
  Vector2 circle_center() {
    return Vector2{pos.x + GateWidth,
                   static_cast<float>(pos.y + GateHeight / 2.0)};
  }
  float circle_radius() { return GateHeight / 2.0; }
  Circle cir() { return Circle{circle_center(), circle_radius()}; }
  bool collision_point(Vector2 point) override {
    return CheckCollisionPointRec(point, rect()) ||
           CheckCollisionPointCircle(point, circle_center(), circle_radius());
  }

  // Not gate class
};

class NotGate : public gate {

  static constexpr float GateHeight = 30;
  static constexpr float GateWidth = 50;

public:
  NotGate(Vector2 pos = {0}) : Node2d(pos), gate(GateWidth, GateHeight, 1, 1) {
    draw_event.add_link([this]() { this->draw(); });
  }
  Vector2 point1() { return pos; }
  Vector2 point2() { return {pos.x, pos.y + GateHeight}; }
  Vector2 point3() { return {pos.x + GateWidth, pos.y + GateHeight / 2.0f}; }
  std::vector<Vector2> points() { return {point1(), point2(), point3()}; }
  void draw() { DrawTriangle(point1(), point2(), point3(), RED); }
  bool collision_point(Vector2 point) override {
    return CheckCollisionPointTriangle(point, point1(), point2(), point3());
  }
  void evaluate() override {
	gateoutputpoints[0]->boolean_state=!gateinputpoints[0]->boolean_state;
  }
};

// Switch
class Switch : public DummyRectObject {
public:
  Switch(Vector2 pos) : Node2d(pos), DummyRectObject("Switch off") {}
};

// Bulb
int main() {
  {
    const u32 ScreenWidth = 800;
    const u32 ScreenHeight = 600;
    InitWindow(ScreenWidth, ScreenHeight, "Logic gate simulator");
  }

  SetTargetFPS(60);
  AndGate a1 = AndGate({20, 50});
  AndGate a2 = AndGate({50, 60});
  NotGate n1 = NotGate({10, 30});
  ObjectSet<object> obj_group;
  obj_group.add(&a1);
 // obj_group.add(&a2);
  obj_group.add(&n1);
  obj_group.add(&Spline::splines);
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
