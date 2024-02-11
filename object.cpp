#include "./basic_template.cpp"
#include <vector>
#include <functional>
#include <set>
#include <iostream>
#include <raylib.h> 

// Forward declaration of classes
class Spline;
class gatepoint;
class object;
class Node2d;
class clickable;
class draggable;
class DummyRectObject;

// Declaration of event class
class event {
public:
    event() : events() {}

    void add_link(std::function<void()> func) { events.push_back(func); }
    void trigger_event() {
        for (auto x : events) {
            x();
        }
    }

private:
    std::vector<std::function<void()>> events;
};

// Declaration of manager namespace
namespace manager {
    static u64 frame = 0;
    extern void onready();
    extern void lastdrawing();

    namespace Debug {
        extern std::string error;
        void add_text(std::string err);
    }
}

// Definition of manager namespace
namespace manager {
    void onready() { frame++; }
    void lastdrawing() { DrawText(Debug::error.c_str(), 5, 5, 11, RED); }
}

// Definition of Debug namespace
namespace manager::Debug {
    std::string error = std::string();
    void add_text(std::string err) {
        error.append("\n");
        error.append(err);
    }
}

// Declaration of object class
class object {
public:
    static std::set<object *> objects;
    object() { objects.insert(this); }
    virtual ~object() { objects.erase(objects.find(this)); }
    static void onready();
    static void onupdate();
    static void ondraw();
    static void oninputevent();
    event ready_event;
    event update_event;
    event draw_event;
    event input_event;
};

// Definition of object class member functions
void object::onready() {
    for (auto x : objects) {
        x->ready_event.trigger_event();
    }
}
void object::onupdate() {
    for (auto x : objects) {
        x->update_event.trigger_event();
    }
}
void object::ondraw() {
    for (auto x : objects) {
        x->draw_event.trigger_event();
    }
}
void object::oninputevent() {
    for (auto x : objects) {
        x->input_event.trigger_event();
    }
}
std::set<object*> object::objects = std::set<object*>();

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
    event on_click_pressed;
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
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
        current_selected == nullptr && collision_point(GetMousePosition())) {
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
DummyRectObject::DummyRectObject(Vector2 pos, const char *text) : Node2d(pos), text(text) {}
Rectangle DummyRectObject::rect() {
    return {pos.x, pos.y, width, height};
}
void DummyRectObject::draw() {
    DrawRectangleV(pos, {width, height}, col);
    DrawText(text, pos.x, pos.y, 11, RED);
}
bool DummyRectObject::collision_point(Vector2 point) {
    return CheckCollisionPointRec(point, rect());
}
class gatepoint : public clickable {
public:
    enum State {
        outgoing,
        ingoing,
    };
    gatepoint(Circle cir);
    Vector2 center();
    bool collision_point(Vector2 point) override;
    void on_click_pressed();

private:
    Circle cir;
    std::vector<Spline*> connected_spline;
    State state;
};

// Declaration of Spline class
class Spline : virtual object {
	const float SplineThickness=3.0;
	const Color SplineColor=GRAY;
public:
    static std::set<Spline *> splines;
    gatepoint *const gatepoint1;
    gatepoint *const gatepoint2;
    Spline(gatepoint *gatepoint1, gatepoint *gatepoint2);
    ~Spline();
    void draw();
};

// Definition of Spline class member functions
std::set<Spline *> Spline::splines;
Spline::Spline(gatepoint *gatepoint1, gatepoint *gatepoint2) : gatepoint1(gatepoint1), gatepoint2(gatepoint2) {
    draw_event.add_link([this]() { this->draw(); });
    splines.insert(this);
}
Spline::~Spline() {
    auto x = splines.find(this);
}
void Spline::draw() {
    if (gatepoint1 == nullptr && gatepoint2 == nullptr) {
        std::cerr << "all points are null ptr";
        return;
    }
    auto point1 = gatepoint1 == nullptr ? GetMousePosition() : gatepoint1->center();
    auto point2 = gatepoint2 == nullptr ? GetMousePosition() : gatepoint2->center();
    DrawLineBezier(point1, point2, SplineThickness, SplineColor);
}

// Definition of gatepoint class

// Definition of gatepoint class member functions
gatepoint::gatepoint(Circle cir) : cir(cir) {
    input_event.add_link([this]() { this->on_click_pressed(); });
}
Vector2 gatepoint::center() {
    return cir.center;
}
bool gatepoint::collision_point(Vector2 point) {
    return CheckCollisionPointCircle(point, cir);
}
void gatepoint::on_click_pressed() {
    if (state == ingoing && !connected_spline.empty()) {
        delete connected_spline[0];
        connected_spline.clear();
    }
    connected_spline.push_back(new Spline(this, nullptr));
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
Rectangle Rect::rect() {
    return Rectangle{pos.x, pos.y, width, height};
}
