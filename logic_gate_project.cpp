#include "basic_ecs.cpp"
#include <raylib.h>
const u32 ScreenWidth = 800;
const u32 ScreenHight = 600;
namespace gates {
namespace andgatepoint {
const u32 rectheight = 30;
const u32 rectwidth = 50;
} // namespace andgatepoint
class draggable {
  bool pressing = false;
  virtual bool point_detection(rlib::Vector2 point) = 0;
  virtual void when_pressing() = 0;
  rlib::Vector2 mouse_position = {0};
  void update() {
    if (rlib::IsMouseButtonDown(rlib::MOUSE_BUTTON_LEFT) &&
        point_detection(rlib::GetMousePosition())) {
      if (!pressing) {
        mouse_position = rlib::GetMousePosition();
      }
      pressing = true;
    } else {
      pressing = false;
    }
    if (pressing) {
      when_pressing();
    }
  }
};
class andgate : public ecs::object, draggable {
  rlib::Vector2 pos = {0};
  u8 thickness = 2;

public:
  andgate() : pos(), thickness(5) {}
  rlib::Rectangle rect() {
    using namespace andgatepoint;
    rlib::Rectangle rect = {pos.x, pos.y, rectwidth, rectheight};
    return rect;
  }
  // namespace point
  void draw() override {
    using namespace rlib;
    using namespace andgatepoint;
    Rectangle rect = this->rect();
    DrawRectangleRec(rect, rlib::RED);
    DrawCircle(pos.x + rectwidth, pos.y + rectheight / 2.0, rectheight / 2.0,
               rlib::RED);
    DrawText("Bezier curve representing AND gate", 10, 10, 20, DARKGRAY);
  }
  bool point_detection(rlib::Vector2 point) override {
    return rlib::CheckCollisionPointRec(point, rect());
  }
  void when_pressing() override {
	
  }
};
} // namespace gates

int main() {
  rlib::InitWindow(ScreenWidth, ScreenHight, "Logic Gate Simulator");
  auto t1 = gates::andgate();
  rlib::SetTargetFPS(60);
  ecs::object::allready();
  while (!rlib::WindowShouldClose()) {
    ecs::object::allupdate();
    rlib::BeginDrawing();
    rlib::ClearBackground(rlib::RAYWHITE);
    ecs::object::alldraw();
    rlib::EndDrawing();
  }
}
