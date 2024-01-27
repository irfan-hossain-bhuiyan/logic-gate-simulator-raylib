#pragma once
#include "./basic_template.cpp"
#include <raylib.h>
namespace manager {
static u64 frame = 0;
static bool clicking = false;
class debug(){
	
}

void onready() {
  frame++;
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    clicking = true;
  }
  if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
    clicking = false;
  }
}
} // namespace manager

class object {
public:
  virtual void ready() {}
  virtual void draw() {}
  virtual void update() = 0;
};

class Node2d : public virtual object {
public:
  Vector2 pos;
  Node2d(Vector2 pos) : pos(pos) {}
};

class clickable {
public:
  static u64 working_frame;
  static clickable *current_selected; // The current selected item pointer is in
                                      // the pointer.nullptr for none.
  virtual bool collision_point(Vector2 point) = 0;

  void click_update() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      if (collision_point(GetMousePosition())) {
        current_selected = this;
        working_frame = manager::frame;
      } else if (working_frame != manager::frame) {
        current_selected = nullptr;
      }
    }
  }
};

u64 clickable::working_frame = 0;
clickable *clickable::current_selected = 0;
class draggable : public virtual Node2d, public virtual clickable {
public:
  void dragging() {
    bool first_time = true;
    click_update();
    static Vector2 previous_obj_pos = {0};
    static Vector2 mouse_click_pos = {0};
    if (manager::clicking == true && current_selected == this) {
      if (first_time == true) {
        first_time = false;
        previous_obj_pos = pos;
        mouse_click_pos = GetMousePosition();
      }
      pos = previous_obj_pos + GetMousePosition() - mouse_click_pos;
    }
    else{first_time=true;}
  }
};
class DummyRectObject : public virtual draggable {
  const float height = 50;
  const float width = 30;

public:
  Color col = RED;
  const char *text = "";
  DummyRectObject(const char *text) : text(text) {}
  Rectangle rect() { return {pos.x, pos.y, width, height}; }
  void draw() override {
    DrawRectangleV(pos, {width, height}, col);
    DrawText(text, pos.x, pos.y, 11, RED);
  }
  bool collision_point(Vector2 point) override {
    return CheckCollisionPointRec(point, rect());
  }
};
