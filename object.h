
#include "basic_template.cpp"
#include <vector>
#include <functional>
#include <set>
#include <raylib.h> // Assuming you're using Raylib for DrawText, DrawRectangleV, etc.

class Spline; // Forward declaration

class event {
public:
    event() : events() {}

    void add_link(std::function<void()> func);
    void trigger_event(); 

private:
    std::vector<std::function<void()>> events;
};

namespace manager {
    extern unsigned long long frame;
    extern Spline* selected_bezier;
    namespace Debug {
        extern std::string error;
        void add_text(std::string err);
    }
    void onready();
    void lastdrawing();
}

class object {
public:
    static std::set<object*> objects;

    object();
    virtual ~object();
    static void onready();
    static void onupdate();
    static void ondraw();
    static void oninputevent();

    event ready_event;
    event update_event;
    event draw_event;
    event input_event;
};
std::set<object*> object::objects=std::set<object*>();

class Node2d : public virtual object {
public:
    Vector2 pos;
    Node2d();
    Node2d(Vector2 pos);
};

class clickable; // Forward declaration

class draggable : public virtual Node2d, public virtual clickable {
public:
    static Vector2 previous_obj_pos;
    static Vector2 mouse_click_pos;

    draggable();

    void dragging();
};
Vector2 draggable::previous_obj_pos = {0};
Vector2 draggable::mouse_click_pos = {0};

class DummyRectObject; // Forward declaration

class Rect : public virtual Node2d {
public:
    float height;
    float width;
    Rect(float width, float height) : height(height), width(width) {}

    Rectangle rect() { return Rectangle{pos.x,pos.y,width,height}; };
};

class gatepoint : public clickable {
    enum State {
        outgoing,
        ingoing,
    };

private:
    Circle cir;
    std::vector<Spline*> connected_spline;
    State state;

public:
    gatepoint(Circle cir) : cir(cir) {
        input_event.add_link([this]() { this->on_click_pressed(); });
    };
    Vector2 center() { return cir.center; }
    bool collision_point(Vector2 point) override {
        return CheckCollisionPointCircle(point, cir);
    }
    void on_click_pressed();
};

class Spline : public virtual object {
public:
    static std::set<Spline*> splines;
    gatepoint* const gatepoint1;
    gatepoint* const gatepoint2;
    Spline(gatepoint* gatepoint1, gatepoint* gatepoint2)
        : gatepoint1(gatepoint1), gatepoint2(gatepoint2) {
        draw_event.add_link([this]() { this->draw(); });
        splines.insert(this);
    }
    ~Spline() { auto x = splines.find(this); }
    void draw() {
        if (gatepoint1 == nullptr && gatepoint2 == nullptr) {
            std::cerr << "all points are null ptr";
            return;
        }
        auto point1 =
            gatepoint1 == nullptr ? GetMousePosition() : gatepoint1->center();
        auto point2 =
            gatepoint2 == nullptr ? GetMousePosition() : gatepoint2->center();
        DrawLineBezier(point1, point2, SplineThickness, SplineColor);
    }
};

class clickable : public virtual object {
public:
    event on_click_pressed;
    bool is_clicking = false;
    clickable() {
        update_event.add_link([this]() { this->clickupdate(); });
    }
    virtual bool collision_point(Vector2 point) = 0;

private:
    static unsigned long long working_frame;
    static clickable *current_selected; // the pointer.nullptr for none.
    void clickupdate() {
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
};
unsigned long long clickable::working_frame = 0;
clickable *clickable::current_selected = 0;

class DummyRectObject : public virtual draggable {
    const float height = 50;
    const float width = 30;

public:
    Color col = RED;
    const char *text = "";
    DummyRectObject(const char *text) : text(text) {
        draw_event.add_link([this]() { this->draw(); });
    }
    DummyRectObject(Vector2 pos, const char *text) : Node2d(pos), text(text) {}
    Rectangle rect() { return {pos.x, pos.y, width, height}; }
    void draw() {
        DrawRectangleV(pos, {width, height}, col);
        DrawText(text, pos.x, pos.y, 11, RED);
    }
    bool collision_point(Vector2 point) override {
        return CheckCollisionPointRec(point, rect());
    }
};
const float SplineThickness = 3;
const Color SplineColor = GRAY;

#endif /* MYHEADER_H */
