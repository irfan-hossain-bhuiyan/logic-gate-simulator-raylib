#include "./object.cpp"
#include <iostream>
#include <raylib.h>

class Spline;
class gate : public draggable, public Rect {
  static constexpr float line_width = 3;
  static constexpr float circle_width = 5;
  static constexpr Color Defaultcolor = BLACK;
  static constexpr float line_height = 10;

public:
  class gatepoint : public clickable, public Node2d {
    const float radius = 4;
    const Color color = BLACK;

  public:
    bool boolean_state;
    enum State {
      outgoing,
      ingoing,
    };
    gatepoint(State state);

    bool collision_point(Vector2 point) override;
    void on_click();
    Circle cir();
    void draw();
    void update_boolean_state();

  private:
    ObjectSet<Spline> connected_spline;
    State state;
  };

  void update() {
    Rectangle rec = rect();
    Vector2 pos = {rec.x, rec.y};
    float segment_height = rec.height / (input_point + 1);
    for (int i = 1; i <= input_point; i++) {
      auto point1 = pos + Vector2{0, segment_height * i};
      auto point2 = point1 + Vector2{-line_height, 0};
      gateinputpoints[i - 1]->pos = point2;
    }
    segment_height = rec.height / (output_point + 1);
    for (int i = 1; i <= output_point; i++) {
      auto point1 = pos + Vector2{rec.width, segment_height * i};
      auto point2 = point1 + Vector2{line_height, 0};
      gateoutputpoints[i - 1]->pos = point2;
    }
    boolean_update();
  }

  virtual void evaluate(){};

  gate(float width, float height, i32 input_point = 2, i32 output_point = 1)
      : input_point(input_point), output_point(output_point),
        Rect(width, height) {
    gateinputpoints.reserve(input_point);
    gateoutputpoints.reserve(output_point);
    range(i, 0, input_point) {
      gateinputpoints.add(new gatepoint(gatepoint::ingoing));
    }
    range(i, 0, output_point) {
      gateoutputpoints.add(new gatepoint(gatepoint::outgoing));
    }
    gateinputpoints.link_to_object(this);
    gateoutputpoints.link_to_object(this);
    update_event.add_link([this]() { this->update(); });
  }
protected:
  ObjectVec<gate::gatepoint> gateinputpoints;
  ObjectVec<gate::gatepoint> gateoutputpoints;
private:
  i32 input_point = 0;
  i32 output_point = 0;
  void boolean_update(){
	for(auto &x:gateinputpoints){x->update_boolean_state();}
	evaluate();
	for(auto &x:gateoutputpoints){x->update_boolean_state();}
  }
};

// Declaration of Spline class
class Spline : public virtual object {
  static constexpr float SplineThickness = 3.0;
  static constexpr Color SplineNegativeColor = GRAY;
  static constexpr Color SplinePositiveColor = RED;

public:
  static ObjectSet<Spline> splines;
  static Spline *drawing_spline;
  gate::gatepoint *outgoing_point;
  gate::gatepoint *ingoing_point;
  bool is_on = false;
  Spline(gate::gatepoint *outgoing_point, gate::gatepoint *ingoing_point);
  void draw();
};

// Definition of Spline class member functions
ObjectSet<Spline> Spline::splines = ObjectSet<Spline>();
Spline *Spline::drawing_spline = nullptr;
Spline::Spline(gate::gatepoint *gatepoint1, gate::gatepoint *gatepoint2)
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
  DrawLineBezier(point1, point2, SplineThickness, this->is_on?SplinePositiveColor:SplineNegativeColor);
}

// Definition of gatepoint class

// Definition of gatepoint class member functions
gate::gatepoint::gatepoint(State state) : state(state) {
  Debug::add_text("constructor called.");
  on_click_pressed.add_link([this]() { this->on_click(); });
  draw_event.add_link([this]() { this->draw(); });
}
void gate::gatepoint::draw() {
  DrawCircleCir(cir(), color);
  DrawLineV(pos,
            pos + Vector2{(state == ingoing ? 1 : -1) * gate::line_height, 0},
            color);
}
void gate::gatepoint::update_boolean_state() {
  if (this->state == ingoing) {
    this->boolean_state =
        this->connected_spline.empty() ? false : connected_spline.head()->is_on;
  }
  if (this->state == outgoing) {
    for (auto x : connected_spline) {
      x->is_on = this->boolean_state;
    }
  }
}
Circle gate::gatepoint::cir() { return Circle{pos, radius}; }
bool gate::gatepoint::collision_point(Vector2 point) {
  return CheckCollisionPointCircle(point, cir());
}
void gate::gatepoint::on_click() {
  // remove the Spline if there is one in the
  // ingoing node.
  // not necessary for the outgoing node because you can have multiple node from
  // the outgoing.
  if (state == ingoing && !connected_spline.empty()) {
    delete connected_spline.head();
  }

  if (Spline::drawing_spline == nullptr) {

    auto ptr = state == ingoing ? new Spline(nullptr, this)
                                : new Spline(this, nullptr);
    connected_spline.add(ptr);
    Spline::drawing_spline = ptr;
  }
  if (Spline::drawing_spline != nullptr) {
    if (state == ingoing && Spline::drawing_spline->ingoing_point == nullptr) {
      // They are doubly linked.You need to coonect pointer of the spline to
      // gatepointer and gatepointer to the spline. There are duplication in
      // here.
      Spline::drawing_spline->ingoing_point = this;
      connected_spline.add(Spline::drawing_spline);
      Spline::drawing_spline = nullptr;

    } else if (state == outgoing &&
               Spline::drawing_spline->outgoing_point == nullptr) {
      // There are duplication in here.
      Spline::drawing_spline->outgoing_point = this;
      connected_spline.add(Spline::drawing_spline);
      Spline::drawing_spline = nullptr;
    }
  }
}