#include "./basic_template.cpp"
#include "./object.cpp"
#include <raylib.h>
#include <set>
#include <vector>
// const region
// end region

// declaration
struct AndGate;
class object;
// declaration end
//
// And Gate class
class AndGate : public draggable {
  const float AndGateHeight = 30;
  const float AndGateWidth = 50;

public:
  AndGate(Vector2 pos = {0}) : Node2d(pos) {draw_event.add_link([this](){this->draw();});}

  void draw() {
    DrawRectangleRec(rect(), RED);
    DrawCircleCir(cir(), RED);
    if (is_clicking) {
      DrawRectangleLinesEx(rect(), 3, BLACK);
    }
  }
  // functions
private:
  Rectangle rect() {
    return Rectangle{pos.x, pos.y, AndGateWidth, AndGateHeight};
  }
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
class NotGate : public draggable {
  const float NotGateHeight = 30;
  const float NotGateWidth = 50;

public:
  NotGate(Vector2 pos = {0}) : Node2d(pos) {draw_event.add_link([this](){this->draw();});}
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
};

// Switch
class Switch : public DummyRectObject {
public:
  Switch(Vector2 pos) : Node2d(pos), DummyRectObject("Switch off") {}
};

// Bulb
int main() {
  static std::set<object *> gameobjects = std::set<object *>();
  {
    const u32 ScreenWidth = 800;
    const u32 ScreenHeight = 600;
    InitWindow(ScreenWidth, ScreenHeight, "Logic gate simulator");
  }

  SetTargetFPS(60);
  AndGate a1 = AndGate({20, 50});
  AndGate a2 = AndGate({50, 60});
  NotGate n1 = NotGate({10, 30});
  gameobjects.insert(&a1);
  gameobjects.insert(&a2);
  gameobjects.insert(&n1);
  for (auto gameobject : gameobjects) {
    gameobject->ready_event.trigger_event();
  }
  while (!WindowShouldClose()) {
    manager::onready(); // For a data to the frame number it is currently on.
    for (auto gameobject : gameobjects) {
      gameobject->input_event.trigger_event();
    }
    for (auto gameobject : gameobjects) {
      gameobject->update_event.trigger_event();
    }
    BeginDrawing();
    ClearBackground(RAYWHITE);
    for (auto gameobject : gameobjects) {
      gameobject->draw_event.trigger_event();
    }
    manager::lastdrawing();
    EndDrawing();
  }
}
