#include "./basic_template.cpp"
#include "./gate.cpp"
#include "./object.cpp"
#include <raylib.h>
#include <vector>
// const region
// end region
// declaration
class AndGate;
// declaration end

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
    DrawText("And",pos.x+5,pos.y+10,11,BLACK);
  }
  void evaluate() override {
    this->gateoutputpoints[0]->boolean_state =
        this->gateinputpoints[0]->boolean_state &&
        this->gateinputpoints[1]->boolean_state;
  }
  // functions
private:
  //  Vector2 circle_center() {
  //    return Vector2{pos.x + GateWidth,
  //                   static_cast<float>(pos.y + GateHeight / 2.0)};
  //  }
  //  float circle_radius() { return GateHeight / 2.0; }
  //  Circle cir() { return Circle{circle_center(), circle_radius()}; }
  bool collision_point(Vector2 point) override {
    return CheckCollisionPointRec(point, rect());
    //          CheckCollisionPointCircle(point, circle_center(),
    //          circle_radius());
  }

  // Not gate class
};
class OrGate : public gate {
  static constexpr float GateHeight = 30;
  static constexpr float GateWidth = 50;

public:
  OrGate(Vector2 pos = {0}) : Node2d(pos), gate(GateWidth, GateHeight, 2, 1) {
    draw_event.add_link([this]() { this->draw(); });
  }

  void draw() {
    DrawRectangleRec(rect(), RED);
    if (is_clicking) {
      DrawRectangleLinesEx(rect(), 3, BLACK);
    }
    DrawText("Or",pos.x+5,pos.y+10,11,BLACK);
  }
  void evaluate() override {
    this->gateoutputpoints[0]->boolean_state =
        this->gateinputpoints[0]->boolean_state ||
        this->gateinputpoints[1]->boolean_state;
  }
  // functions
private:
  //  Vector2 circle_center() {
  //    return Vector2{pos.x + GateWidth,
  //                   static_cast<float>(pos.y + GateHeight / 2.0)};
  //  }
  //  float circle_radius() { return GateHeight / 2.0; }
  //  Circle cir() { return Circle{circle_center(), circle_radius()}; }
  bool collision_point(Vector2 point) override {
    return CheckCollisionPointRec(point, rect());
    //          CheckCollisionPointCircle(point, circle_center(),
    //          circle_radius());
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
    gateoutputpoints[0]->boolean_state = !gateinputpoints[0]->boolean_state;
  }
};

// Switch
class Light : public gate {
  static constexpr float GateHeight = 30;
  static constexpr float GateWidth = 50;

public:
  Light(Vector2 pos = {0}) : Node2d(pos), gate(GateWidth, GateHeight, 1, 0) {
    draw_event.add_link([this]() { this->draw(); });
  }

  void draw() {
    DrawRectangleRec(rect(), gateinputpoints[0]->boolean_state ? RED : BLUE);
    if (is_clicking) {
      DrawRectangleLinesEx(rect(), 3, BLACK);
    }
    DrawText("L",pos.x+5,pos.y+10,15,BLACK);
  }
  // functions
private:
  //  Vector2 circle_center() {
  //    return Vector2{pos.x + GateWidth,
  //                   static_cast<float>(pos.y + GateHeight / 2.0)};
  //  }
  //  float circle_radius() { return GateHeight / 2.0; }
  //  Circle cir() { return Circle{circle_center(), circle_radius()}; }
  bool collision_point(Vector2 point) override {
    return CheckCollisionPointRec(point, rect());
    //          CheckCollisionPointCircle(point, circle_center(),
    //          circle_radius());
  }

  // Not gate class
};
class Switch : public gate {
  static constexpr float GateHeight = 30;
  static constexpr float GateWidth = 50;
  bool is_on = false;

public:
  Switch(Vector2 pos = {0}) : Node2d(pos), gate(GateWidth, GateHeight, 0, 1) {
    draw_event.add_link([this]() { this->draw(); });
    on_click_pressed.add_link([this]() { this->is_on = !this->is_on; });
  }

  void draw() {
    DrawRectangleRec(rect(), is_on ? GREEN : RED);
    if (is_clicking) {
      DrawRectangleLinesEx(rect(), 3, BLACK);
    }
    DrawText("S",pos.x+5,pos.y+10,15,BLACK);
  }
  void evaluate()override{
	gateoutputpoints[0]->boolean_state=is_on;
  }
  // functions
private:
  //  Vector2 circle_center() {
  //    return Vector2{pos.x + GateWidth,
  //                   static_cast<float>(pos.y + GateHeight / 2.0)};
  //  }
  //  float circle_radius() { return GateHeight / 2.0; }
  //  Circle cir() { return Circle{circle_center(), circle_radius()}; }
  bool collision_point(Vector2 point) override {
    return CheckCollisionPointRec(point, rect());
    //          CheckCollisionPointCircle(point, circle_center(),
    //          circle_radius());
  }

  // Not gate class
};

// Bulb

