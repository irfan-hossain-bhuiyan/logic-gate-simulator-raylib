#include "./searchbar.cpp"
#include <raylib.h>
class HoverBar:public object {
private:
  bool active = false;
  SearchBar sb;

public:
  event<const std::string &> on_enter_press;
  HoverBar() {
    sb.on_select.add_link([this](auto x) {
      this->active = false;
      on_enter_press.trigger_event(x);
    });
    update_event.add_link([this](){this->update();});
  }
  void update() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
      active = true;
      sb.pos = GetMousePosition();
    }
    if (active) {
      sb.update_event.trigger_event();
    }
  }
};
