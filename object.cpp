#include "./basic_template.cpp"
#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <queue>
#include <raylib.h>
#include <set>
#include <unordered_map>
#include <vector>

// Forward declaration of classes
class Spline;
class gatepoint;
class object;
class Node2d;
class clickable;
class draggable;
class DummyRectObject;
template <typename T> class ObjectSet;

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
void onready();
void lastdrawing();

} // namespace manager
namespace Debug {
const u32 max_size_ = 16;
std::queue<std::string> queue_ = std::queue<std::string>();
void add_text(const std::string &text) {
  queue_.push(text);
  if (queue_.size() > max_size_) {
    queue_.pop(); // Remove the oldest string if the queue size exceeds the
                  // maximum size
  }
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
void onready() { frame++; }
void lastdrawing() {
  DrawText(Debug::get_combined_text().c_str(), 5, 5, 11, RED);
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
    on_death.trigger_event(this);
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
  void on_death_add_link(std::function<void(object *)> func) {
    on_death.add_link(func);
  }

private:
  event<object *> on_death;
  object *linked_object = nullptr;
  std::array<u32, 3> linked_id = {0};
};
template <typename T> class ObjectSet : public object {
  std::set<T *> objects;

public:
  ObjectSet() {
    ready_event.add_link([this]() { this->ready(); });
    update_event.add_link([this]() { this->update(); });
    draw_event.add_link([this]() { this->draw(); });
  }
  ObjectSet(std::initializer_list<T *> objs)
      : objects(objs.begin(), objs.end()) {
    ObjectSet();
  }
  void add(T *obj) {
    objects.insert(obj);
    obj->on_death_add_link([this](auto x) { this->objects.erase(dynamic_cast<T*>(x));});
  }
  void remove(object *obj) { objects.erase(obj); }
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
  // void all_input() {
  //   for (auto x : objects) {
  //     x->input_event.trigger_event();
  //   }
  // }
};
template <typename T> class ObjectVec : public object {

public:
  std::vector<std::unique_ptr<T>> ptr_array;
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
  void add(T *element) { this->ptr_array.push_back(std::unique_ptr<T>(element)); }
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
};

// Definition of object class member functions

// Declaration of Node2d class
class Node2d : public virtual object {
public:
  Vector2 pos;
  Node2d() : pos() {}
  Node2d(Vector2 pos) : pos(pos) {}
};

// Declaration of clickable class
class clickable : public virtual object {
public:
  event<> on_click_pressed;
  bool is_clicking = false;
  clickable();
  virtual bool collision_point(Vector2 point) = 0;

private:
  static u64 working_frame;
  static clickable *current_selected; // the pointer.nullptr for none.
  void clickupdate();
};

// Definition of clickable class member functions
clickable::clickable() {
  update_event.add_link([this]() { this->clickupdate(); });
}
u64 clickable::working_frame = 0;
clickable *clickable::current_selected = 0;
void clickable::clickupdate() {
  if (working_frame != manager::frame) {
    current_selected = nullptr;
  }
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && current_selected == nullptr &&
      collision_point(GetMousePosition())) {
    current_selected = this;
    working_frame = manager::frame;
    on_click_pressed.trigger_event();
    is_clicking = true;
  }
  if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    is_clicking = false;
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
  if (is_clicking) {
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
class gatepoint : public clickable, public Node2d {
  const float radius = 4;
  const Color color = BLACK;

public:
  enum State {
    outgoing,
    ingoing,
  };
  gatepoint(State state);

  bool collision_point(Vector2 point) override;
  void on_click();
  Circle cir();
  void draw();

private:
  std::vector<Spline *> connected_spline;
  State state;
};

// Declaration of Spline class
class Spline :public virtual object {
  const float SplineThickness = 3.0;
  const Color SplineColor = GRAY;

public:
  static ObjectSet<Spline> splines;
  static Spline *drawing_spline;
  gatepoint *outgoing_point;
  gatepoint *ingoing_point;
  Spline(gatepoint *outgoing_point, gatepoint *ingoing_point);
  void draw();
};

// Definition of Spline class member functions
ObjectSet<Spline> Spline::splines=ObjectSet<Spline>();
Spline *Spline::drawing_spline = nullptr;
Spline::Spline(gatepoint *gatepoint1, gatepoint *gatepoint2)
    : outgoing_point(gatepoint1), ingoing_point(gatepoint2) {
  draw_event.add_link([this]() { this->draw(); });
  splines.add(this);
}
void Spline::draw() {
  if (outgoing_point == nullptr && ingoing_point == nullptr) {
    std::cerr << "all points are null ptr";
    return;
  }
  auto point1 =
      outgoing_point == nullptr ? GetMousePosition() : outgoing_point->pos;
  auto point2 =
      ingoing_point == nullptr ? GetMousePosition() : ingoing_point->pos;
  DrawLineBezier(point1, point2, SplineThickness, SplineColor);
}

// Definition of gatepoint class

// Definition of gatepoint class member functions
gatepoint::gatepoint(State state) : state(state) {
  Debug::add_text("constructor called.");
  on_click_pressed.add_link([this]() { this->on_click(); });
  draw_event.add_link([this]() { this->draw(); });
}
void gatepoint::draw() { DrawCircleCir(cir(), color); }
Circle gatepoint::cir() { return Circle{pos, radius}; }
bool gatepoint::collision_point(Vector2 point) {
  return CheckCollisionPointCircle(point, cir());
}
void gatepoint::on_click() {
  // remove the Spline if there is one in the
  // ingoing node.
  // not necessary for the outgoing node because you can have multiple node from
  // the outgoing.
  if (state == ingoing && !connected_spline.empty()) {
    delete connected_spline[0];
    connected_spline.clear();
  }

  if (Spline::drawing_spline == nullptr) {

    auto ptr = state == ingoing ? new Spline(nullptr, this)
                                : new Spline(this, nullptr);
    connected_spline.push_back(ptr);
    Spline::drawing_spline = ptr;
  }
  if (Spline::drawing_spline != nullptr) {
    if (state == ingoing && Spline::drawing_spline->ingoing_point == nullptr) {
      // They are doubly linked.You need to coonect pointer of the spline to
      // gatepointer and gatepointer to the spline. There are duplication in
      // here.
      Spline::drawing_spline->ingoing_point = this;
      connected_spline.push_back(Spline::drawing_spline);
      Spline::drawing_spline = nullptr;

    } else if (state == outgoing &&
               Spline::drawing_spline->outgoing_point == nullptr) {
      // There are duplication in here.
      Spline::drawing_spline->outgoing_point = this;
      connected_spline.push_back(Spline::drawing_spline);
      Spline::drawing_spline = nullptr;
    }
  }
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
