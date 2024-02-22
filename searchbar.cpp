#include "./basic_template.cpp"
#include "./object.cpp"
#include <algorithm>
#include <cstring>
#include <raygui.h>
#include <raylib.h>
#include <string>
#include <vector>

class InputBox : public virtual object, public Rect {
public:
  static constexpr float width = 100;
  static constexpr float height = 30;
  static constexpr int text_size = 64;
  char text[text_size];
  event<> on_enter;
  event<> on_text_update;
  InputBox(Vector2 pos) : Node2d(pos), Rect(width, height) {
    draw_event.add_link([this]() { this->draw(); });
  }

private:
  char text_previous[text_size];
  void draw() {
    if (strcmp(text, text_previous) != 0) {
      strcpy(text_previous, text);
      on_text_update.trigger_event();
    }
    if (GuiTextBox(rect(), text, text_size, true)) {
      on_enter.trigger_event();
    }
  }
};
class MenuBox : public virtual object, public Rect {
public:
  static constexpr float width = 100;
  static constexpr float height = 30;
  event<std::string> on_select;
  std::vector<std::string> texts;
  MenuBox(Vector2 pos) : Node2d(pos), Rect(width, height) {
    draw_event.add_link([this]() { this->draw(); });
  }

private:
  int selected = -1;
  void draw() {
    if (GuiDropdownBox(rect(), vectorToString(texts).c_str(), &selected,
                       true)) {
      if (-1 < selected && selected < texts.size()) {
        on_select.trigger_event(texts[selected]);
      }
    }
  }
};
class SearchBar : public virtual object, public Node2d {
private:
  InputBox ib;
  MenuBox mb;

public:
  std::vector<std::string> texts;
  event<std::string> on_select;
  SearchBar(Vector2 pos)
      : Node2d(pos), ib(pos), mb(pos + Vector2{0, InputBox::height}) {
    this->draw_event.add_link([this]() {
      ib.draw_event.trigger_event();
      mb.draw_event.trigger_event();
    });
    ib.on_enter.add_link([this]() { this->on_enter_press(); });
    ib.on_text_update.add_link([this]() { this->on_text_input(); });
    mb.on_select.add_link(
        [this](std::string select) { this->on_selection(select); });
    mb.texts = texts;
  }

private:
  void on_text_input() {
    if (ib.text[0] == '\0') {
      mb.texts = texts;
    } else {
      mb.texts = fuzzySearch(std::string(ib.text), this->texts);
    }
  }
  void on_enter_press() { on_select.trigger_event(std::string(ib.text)); }
  void on_selection(std::string select) { strcpy(ib.text, select.c_str()); }
};
