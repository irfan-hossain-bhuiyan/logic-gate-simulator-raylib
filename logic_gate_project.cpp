#include "basic_ecs.cpp"
#include <raylib.h>
const u32 ScreenWidth = 800;
const u32 ScreenHight = 600;
class pointcheck{
	virtual bool pointcollision(rlib::Vector2 point)=0;
};
namespace gates {
namespace andgatepoint {
const u32 rectheight = 30;
const u32 rectwidth = 50;
} // namespace andgatepoint
class andgate : public ecs::object {
  rlib::Vector2 pos = {0};
  u8 thickness = 2;

public:
  andgate() : pos(), thickness(5) {}
  // namespace point
  void draw() override {
    using namespace rlib;
    using namespace andgatepoint;
    Rectangle rect = {pos.x, pos.y, rectwidth, rectheight};
    DrawRectangleRec(rect, rlib::RED);
    DrawCircle(pos.x + rectwidth, pos.y + rectheight / 2.0, rectheight/ 2.0,
               rlib::RED);
    DrawText("Bezier curve representing AND gate", 10, 10, 20, DARKGRAY);
  }
};
} // namespace gates

int main() {
  rlib::InitWindow(ScreenWidth, ScreenHight, "Logic Gate Simulator");
  auto t1 = gates::andgate();
  rlib::SetTargetFPS(60);
  ecs::object::allready();
  while (!rlib::WindowShouldClose()) {
    rlib::ClearBackground(rlib::RAYWHITE);
    rlib::BeginDrawing();
    ecs::object::alldraw();
    rlib::EndDrawing();
  }
}
