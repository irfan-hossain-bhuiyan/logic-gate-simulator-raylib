// #include "basic_ecs.cpp"
#include "./basic_template.cpp"
#include <raylib.h>
#include <set>
#include <vector>
// const region
// end region

// declaration
struct AndGate;
class object;
// declaration end
class object {
public:
  virtual void ready() {}
  virtual void draw() {}
  virtual void update() {}
};
class Node2d : public virtual object {
public:
  Vector2 pos;
  Node2d(Vector2 pos) : pos(pos) {}
};
class draggable : public virtual Node2d {
public:
  bool clicking = false;
  virtual bool collision_point(Vector2 point) = 0;
  void dragging() {

    static draggable *current_select = nullptr;
    static Vector2 previous_obj_pos = {0};
    static Vector2 mouse_click_pos = {0};
    if (current_select == nullptr) {
      if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
          collision_point(GetMousePosition())) {
        if (clicking == false) {
          previous_obj_pos = pos;
          mouse_click_pos = GetMousePosition();
          current_select = this;
        }
        clicking = true;
      }
    }
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
      clicking = false;
      current_select = nullptr;
    }
    if (clicking == true) {
      pos = previous_obj_pos + GetMousePosition() - mouse_click_pos;
    }
  }
};
class AndGate : public draggable {
  const float AndGateHeight = 30;
  const float AndGateWidth = 50;

public:
  AndGate(Vector2 pos = {0}) : Node2d(pos) {}

  void update() override { dragging(); }
  void draw() override {
    DrawRectangleRec(rect(), RED);
    DrawCircleCir(cir(), RED);
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
};
class NotGate : public draggable {
  const float NotGateHeight = 30;
  const float NotGateWidth = 50;

public:
  NotGate(Vector2 pos = {0}) : Node2d(pos) {}
  Vector2 point1() { return pos; }
  Vector2 point2() { return {pos.x, pos.y + NotGateHeight}; }
  Vector2 point3() {
    return {pos.x + NotGateWidth, pos.y + NotGateHeight / 2.0f};
  }
  std::vector<Vector2> points() { return {point1(), point2(), point3()}; }
  void draw() override { DrawTriangle(point1(), point2(), point3(), RED); }
  bool collision_point(Vector2 point) override {
    return CheckCollisionPointTriangle(point, point1(), point2(), point3());
  }
  void update() override { dragging(); }
};
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
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    for (auto gameobject : gameobjects) {
      gameobject->update();
      gameobject->draw();
    }

    EndDrawing();
  }
}
