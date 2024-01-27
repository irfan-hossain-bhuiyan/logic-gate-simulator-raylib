#include <iostream>
#include <set>

namespace rlib {
#include "raylib.h"
}
#include "./basic_template.cpp"
namespace ecs {
class object {
private:
  static std::set<object *> objects;
  u32 id;

protected:
  object() {
    id = objects.size();
    objects.insert(this);
  }

protected:
  virtual ~object() {
    auto it = objects.find(this);
    if (it == objects.end()) {
      std::cerr << "The object pointer was already removed.";
    } else {
      objects.erase(it);
    }
  }
  virtual void update() {}
  virtual void draw() {}
  virtual void ready() {}

public:
  static void allready() {
    for (auto obj : objects) {
      obj->ready();
    }
  }
  static void alldraw() {
    rlib::BeginDrawing();

    for (auto obj : objects) {
      obj->draw();
    }
    rlib::EndDrawing();
  }
  static void allupdate() {
    for (auto obj : objects) {
      obj->draw();
    }
  }
};
std::set<object *> object::objects = std::set<object *>();
} // namespace ecs
