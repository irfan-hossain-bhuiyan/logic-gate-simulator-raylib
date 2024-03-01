#pragma once
#include "./basic_template.cpp"
#include <algorithm>
#include <array>
#include <functional>
#include <iterator>
#include <memory>
#include <queue>
#include <raylib.h>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
// Forward declaration of classes
class object;
class Node2d;
class clickable;
class draggable;
class DummyRectObject;
template <typename T> class ObjectSet;
template <typename T> struct Objcmp {
  bool operator()(const T* x, const T* y) const {
    return static_cast<const object *>(x) < static_cast<const object *>(y);
  }
};
// Declaration of event class
template <typename... Args> class event {
public:
  event() : events() {}

  u32 add_link(std::function<void(Args...)> func) {
    events[id] = func;
    return id++;
  }
  void remove_link(u32 id) { events.erase(id); }
  void trigger_event(Args... args) {
    for (auto [k, x] : events) {
      x(args...);
    }
  }
  template <typename Collection> void remove_links(const Collection &coll) {
    for (const auto &x : coll) {
      remove_link(x);
    }
  }

private:
  std::unordered_map<u32, std::function<void(Args...)>> events;
  u32 id = 0;
};

// Declaration of manager namespace
namespace manager {
static u64 frame = 0;
void onupdate();
void lastdrawing();

} // namespace manager
namespace Debug {
const u32 max_size_ = 16;
std::stringstream str_;
std::queue<std::string> queue_ = std::queue<std::string>();
void add_text() {
  queue_.push(std::move(str_).str());
  str_.str("");
  if (queue_.size() > max_size_) {
    queue_.pop(); // Remove the oldest string if the queue size exceeds the
                  // maximum size
  }
}
template <typename T, typename... Args> void add_text(T &text, Args &...rest) {
  str_ << text;
  add_text(rest...);
}

std::string get_combined_text() {
  std::string combined_text;
  size_t count = 0;
  std::queue<std::string> temp_queue =
      queue_; // Create a temporary copy to avoid modifying the original queue
  while (!temp_queue.empty() && count < max_size_) {
    combined_text += temp_queue.front();
    combined_text.push_back('\n');
    temp_queue.pop();
    ++count;
  }
  return combined_text;
}

} // namespace Debug
// Definition of manager namespace
namespace manager {
void onupdate() { frame++; }
void lastdrawing() {
  DrawText(Debug::get_combined_text().c_str(), 5, 5, 15, RED);
}
} // namespace manager

// Definition of Debug namespace
// namespace manager::Debug

// Declaration of object class
class object {
public:
  object() {}
  virtual ~object() {
    Debug::add_text("Destructor called.");
    on_death.trigger_event();
  }

public:
  event<> ready_event;
  event<> update_event;
  event<> draw_event;
  // event<> input_event;
  std::array<u32, 3> link_all(std::function<void()> ready_fn,
                              std::function<void()> update_fn,
                              std::function<void()> draw_fn) {
    return std::array<u32, 3>{ready_event.add_link(ready_fn),
                              update_event.add_link(update_fn),
                              draw_event.add_link(draw_fn)};
  }

public:
  void unlink_all(std::array<u32, 3> ids) {
    ready_event.remove_link(ids[0]);
    update_event.remove_link(ids[1]);
    draw_event.remove_link(ids[2]);
  }
  void link_to_object(object *obj) {
    linked_id = obj->link_all([this]() { this->ready_event.trigger_event(); },
                              [this]() { this->update_event.trigger_event(); },
                              [this]() { this->draw_event.trigger_event(); });
    linked_object = obj;
  }
  void unlink_object() {
    if (linked_object == nullptr) {
      return;
    }
    linked_object->unlink_all(linked_id);
    linked_object = nullptr;
  }
  void on_death_add_link(std::function<void()> func) {
    on_death.add_link(func);
  }

private:
  event<> on_death;
  object *linked_object = nullptr;
  std::array<u32, 3> linked_id = {0};
};
template <typename T> class ObjectSet : public object {
  std::set < T *, Objcmp<T> > objects;

public:
  ObjectSet() {
    ready_event.add_link([this]() { this->ready(); });
    update_event.add_link([this]() { this->update(); });
    draw_event.add_link([this]() { this->draw(); });
  }
  ObjectSet(std::initializer_list<T *> objs) {
    ObjectSet();
    for (auto x : objs)
      add(x);
  }
  T *head() { return *objects.begin(); }
  auto begin() { return objects.begin(); }
  auto end() { return objects.end(); }
  void add(T *obj) {
    objects.insert(obj);
    obj->on_death_add_link([this, obj]() {
      auto it = this->objects.find(obj);
      if (it == objects.end()) {
        Debug::add_text("The element isn't here.It got destroyed before.");
        return;
      }
      this->objects.erase(it);
    });
  }
  void ready() {
    for (auto x : objects) {
      x->ready_event.trigger_event();
    }
  }
  void update() {
    for (auto x : objects) {
      x->update_event.trigger_event();
    }
  }
  void draw() {
    for (auto x : objects) {
      x->draw_event.trigger_event();
    }
  }
  bool empty() { return objects.empty(); }
  void clear() { objects.clear(); }
  void delete_obj(object *obj) {
	  auto it=std::lower_bound(begin(),end(),obj,[](auto x,auto y){return static_cast<object*>(x)<static_cast<object*>(y);});
	  if(it==end() || static_cast<object*>(*it)!=obj){return;}
	  delete obj;
   }
  void delete_all() {
    std::vector<T *> temp;
    std::move(objects.begin(), objects.end(), std::back_inserter(temp));
    objects.clear();
    for (auto x : temp)
      delete x;
  }
  // void all_input() {
  //   for (auto x : objects) {
  //     x->input_event.trigger_event();
  //   }
  // }
};
template <typename T> class ObjectVec : public object {

  std::vector<std::unique_ptr<T>> ptr_array;

public:
  ObjectVec() : ptr_array() {
    ready_event.add_link([this]() { this->ready(); });
    update_event.add_link([this]() { this->update(); });
    draw_event.add_link([this]() { this->draw(); });
  }
  ObjectVec(std::initializer_list<T *> ptr) {
    std::transform(ptr.begin(), ptr.end(), std::back_inserter(&ptr_array),
                   [](auto x) { return std::unique_ptr<T>(x); });
    // Duplication here.
    ObjectVec();
  }
  void add(T *element) {
    this->ptr_array.push_back(std::unique_ptr<T>(element));
  }
  void ready() {
    for (auto &x : this->ptr_array) {
      x->ready_event.trigger_event();
    }
  }
  void update() {
    for (auto &x : this->ptr_array) {
      x->update_event.trigger_event();
    }
  }
  void draw() {
    for (auto &x : this->ptr_array) {
      x->draw_event.trigger_event();
    }
  }
  std::unique_ptr<T> &operator[](size_t index) { return ptr_array[index]; }
  void reserve(size_t size) { ptr_array.reserve(size); }
  auto begin() { return ptr_array.begin(); }
  auto end() { return ptr_array.end(); }
};

// Definition of object class member functions

// Declaration of Node2d class
class Node2d {
public:
  Vector2 pos;
  Node2d() : pos() {}
  Node2d(Vector2 pos) : pos(pos) {}
};

// Declaration of clickable class
class clickable : public virtual object {
public:
  static clickable *selected;
  static clickable *current_selected; // the pointer.nullptr for none.
static clickable *previous_selection;
  event<> on_click_pressed;
  event<> click_update;
  clickable();
  virtual bool collision_point(Vector2 point) = 0;

private:
  void clickupdate();
};
clickable* clickable::previous_selection=nullptr;
// Definition of clickable class member functions
clickable::clickable() {
  click_update.add_link([this]() { this->clickupdate(); });
}
clickable *clickable::current_selected = 0;
clickable *clickable::selected = 0;
void clickable::clickupdate() {
  if (collision_point(GetMousePosition()) && selected == nullptr) {
    selected = this;
    on_click_pressed.trigger_event();
  }
}

// Declaration of draggable class
class draggable : public virtual Node2d, public virtual clickable {
public:
  static Vector2 previous_obj_pos;
  static Vector2 mouse_click_pos;
  draggable();
  void dragging();
};

// Definition of draggable class member functions
draggable::draggable() {
  on_click_pressed.add_link([this]() {
    previous_obj_pos = pos;
    mouse_click_pos = GetMousePosition();
  });
  update_event.add_link([this]() { this->dragging(); });
}
Vector2 draggable::previous_obj_pos = {0};
Vector2 draggable::mouse_click_pos = {0};
void draggable::dragging() {
  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
      this == clickable::current_selected) {
    pos = GetMousePosition() - mouse_click_pos + previous_obj_pos;
  }
}

// Declaration of DummyRectObject class
class DummyRectObject : public virtual draggable {
public:
  const float height = 50;
  const float width = 30;
  Color col = RED;
  const char *text = "";
  DummyRectObject(const char *text);
  DummyRectObject(Vector2 pos, const char *text);
  Rectangle rect();
  void draw();
  bool collision_point(Vector2 point) override;
};

// Definition of DummyRectObject class member functions
DummyRectObject::DummyRectObject(const char *text) : text(text) {
  draw_event.add_link([this]() { this->draw(); });
}
DummyRectObject::DummyRectObject(Vector2 pos, const char *text)
    : Node2d(pos), text(text) {}
Rectangle DummyRectObject::rect() { return {pos.x, pos.y, width, height}; }
void DummyRectObject::draw() {
  DrawRectangleV(pos, {width, height}, col);
  DrawText(text, pos.x, pos.y, 11, RED);
}
bool DummyRectObject::collision_point(Vector2 point) {
  return CheckCollisionPointRec(point, rect());
}

// Declaration of Rect class
class Rect : public virtual Node2d {
public:
  float height;
  float width;
  Rect(float width, float height);
  Rectangle rect();
};
// Definition of Rect class member functions
Rect::Rect(float width, float height) : height(height), width(width) {}
Rectangle Rect::rect() { return Rectangle{pos.x, pos.y, width, height}; }
