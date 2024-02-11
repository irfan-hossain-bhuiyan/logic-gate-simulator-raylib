#include "./basic_template.cpp"
#include "./object.cpp"
#include <raylib.h>
#include <vector>
// const region
// end region

// declaration
struct AndGate;
class object;
// declaration end
//
// And Gate class
class gate : public draggable, public Rect {
  const float line_width = 3;
  const float circle_width = 5;
  const float line_height = 10;
  const Color Defaultcolor = BLACK;

private:
  i32 input_point = 0;
  i32 output_point = 0;

public:
  std::vector<bool> inputs;
  std::vector<bool> outputs;
  void draw() {
    Rectangle rec = rect();
    Vector2 pos = {rec.x, rec.y};
    float segment_height = rec.height / (input_point + 1);
    // Draw input lines
    for (int i = 1; i <= input_point; i++) {
      auto point1 = pos + Vector2{0, segment_height * i};
      auto point2 = point1 + Vector2{-line_height, 0};
      DrawLineV(point1, point2, Defaultcolor);
      DrawCircleV(point2, circle_width, Defaultcolor);
    }
    // Draw output line
    segment_height = rec.height / (output_point + 1);
    // Draw input lines
    for (int i = 1; i <= output_point; i++) {
      auto point1 = pos + Vector2{rec.width, segment_height * i};
      auto point2 = point1 + Vector2{line_height, 0};
      DrawLineV(point1, point2, Defaultcolor);
      DrawCircleV(point2, circle_width, Defaultcolor);
    }
  }

public:
  virtual void evaluate(){};
  gate(float width, float height, i32 input_point = 2, i32 output_point = 1)
      : input_point(input_point), output_point(output_point),
        inputs(input_point), outputs(output_point), Rect(width, height) {

    draw_event.add_link([this]() { this->draw(); });
  }
};

const float AndGateHeight = 30;
const float AndGateWidth = 50;
class AndGate : public gate {

public:
  AndGate(Vector2 pos = {0})
      : Node2d(pos), gate(AndGateWidth, AndGateHeight, 2, 1) {
    draw_event.add_link([this]() { this->draw(); });
  }

  void draw() {
    DrawRectangleRec(rect(), RED);
    if (is_clicking) {
      DrawRectangleLinesEx(rect(), 3, BLACK);
    }
  }
  void evaluate() override { outputs[0] = inputs[0] && inputs[1]; }
  // functions
private:
  Vector2 circle_center() {
    return Vector2{pos.x + AndGateWidth,
                   static_cast<float>(pos.y + AndGateHeight / 2.0)};
  }
  float circle_radius() { return AndGateHeight / 2.0; }
  Circle cir() { return Circle{circle_center(), circle_radius()}; }
  bool collision_point(Vector2 point) override {
    return CheckCollisionPointRec(point, rect()) ||
           CheckCollisionPointCircle(point, circle_center(), circle_radius());
  }

  // Not gate class
};

const float NotGateHeight = 30;
const float NotGateWidth = 50;
class NotGate : public gate {

public:
  NotGate(Vector2 pos = {0})
      : Node2d(pos), gate(NotGateWidth, NotGateHeight, 1, 1) {
    draw_event.add_link([this]() { this->draw(); });
  }
  Vector2 point1() { return pos; }
  Vector2 point2() { return {pos.x, pos.y + NotGateHeight}; }
  Vector2 point3() {
    return {pos.x + NotGateWidth, pos.y + NotGateHeight / 2.0f};
  }
  std::vector<Vector2> points() { return {point1(), point2(), point3()}; }
  void draw() { DrawTriangle(point1(), point2(), point3(), RED); }
  bool collision_point(Vector2 point) override {
    return CheckCollisionPointTriangle(point, point1(), point2(), point3());
  }
  void evaluate() override { outputs[0] = !inputs[0]; }
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
  object::onready();
  while (!WindowShouldClose()) {
    manager::onready(); // For a data to the frame number it is currently on.
    object::oninputevent();
    object::onupdate();
    BeginDrawing();
    ClearBackground(RAYWHITE);
    object::ondraw();
    manager::lastdrawing();
    EndDrawing();
  }
}
