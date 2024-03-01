#include "./searchbar.cpp"
#include <raylib.h>
class GateSearchBar : public object {
private:
  bool active = false;
  SearchBar sb;

public:
  event<const std::string &> on_enter_press;
  GateSearchBar(const std::vector<std::string> *texts) : sb({0}, texts) {
    sb.on_select.add_link([this](auto x) {
      this->active = false;
      on_enter_press.trigger_event(x);
    });
    update_event.add_link([this]() { this->update(); });
    draw_event.add_link([this]() { this->draw(); });
  }
  void update() {
    if (active) {
      sb.update_event.trigger_event();
    }
  }
  void draw() {
    if (active) {
      sb.draw_event.trigger_event();
    }
  }
  void enable() {
    this->active = true;
    this->sb.set_pos(GetMousePosition());
  }
};
