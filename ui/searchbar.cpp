#include "../basic_template.cpp"
#include "../object.cpp"
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
class MenuBox : public virtual object, public Node2d {
public:
  static constexpr float option_width = 100;
  static constexpr float option_height = 30;
  std::vector<std::string> texts;
  event<const std::string &> on_select;
  MenuBox(Vector2 pos) : Node2d(pos) {
    draw_event.add_link([this]() { this->draw(); });
  }

private:
  int selected = -1;
  void draw() {
    for (int i = 0; i < texts.size(); i++) {
      if (GuiButton(Rectangle{pos.x, pos.y + i * option_height, option_width,
                              option_height},
                    texts[i].c_str())) {
        on_select.trigger_event(texts[i]);
      }
    }
  }
};
class SearchBar : public virtual object, public Node2d {
private:
  InputBox ib;
  MenuBox mb;

  std::vector<std::string> texts;

public:
  event<std::string> on_select;
  SearchBar(Vector2 pos = {0})
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
  void set_texts(std::vector<std::string> &&text) {
    texts = text;
    on_text_input();
  }
  void push_text(std::string &&text) {
    texts.push_back(text);
    on_text_input();
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
