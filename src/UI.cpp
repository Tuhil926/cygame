#include "cygame.h"
#include <ostream>

Button::Button(SDL_Rect rect, std::string text, Font *font, Color color,
               Color hover_color, Color click_color, void (*on_click)(void *),
               void *arg, Color text_color) {
    this->text = text;
    this->pos = {(float)rect.x, (float)rect.y};
    width = rect.w;
    height = rect.h;
    this->color = color;
    this->default_color = color;
    this->hover_color = hover_color;
    this->click_color = click_color;
    is_colliding = false;
    clicked = false;
    this->on_click = on_click;
    this->arg = arg;
    this->text_color = text_color;
    this->font = font;
    this->rect = rect;
    text_obj = Text(pos + Pos2D{width / 2 + height / 2}, this->text, this->font,
                    this->text_color);
}
void Button::draw(Camera &camera) {
    camera.draw_rect(rect, color);
    if (text.size())
        this->text_obj.draw(camera);
}
// Button::Button() {}
void Button::update(MouseState mouse_state) {
    rect = {(int)pos.x, (int)pos.y, (int)width, (int)height};
    if (collide_rect(rect, {(float)mouse_state.x, (float)mouse_state.y})) {
        if (mouse_state.pressed_left()) {
            if (!clicked && is_colliding) {
                // std::cout << "ff\n";
                if (on_click) {
                    // std::cout << "nice but wtf\n";
                    on_click(arg);
                }
                color = click_color;
            } else
                color = click_color;
        } else
            color = hover_color;
        is_colliding = true;

    } else {
        color = default_color;
        is_colliding = false;
    }
    clicked = mouse_state.pressed_left();
    text_obj.pos = pos + Pos2D{width / 2, height / 2};
    text_obj.text_color = text_color;
    text_obj.text = text;
}

InputBox::InputBox(SDL_Rect rect, std::string text, Font *font, Color color,
                   Color hover_color, Color click_color, Color text_color,
                   int max_len) {
    this->text = text;
    this->pos = {(float)rect.x, (float)rect.y};
    width = rect.w;
    height = rect.h;
    this->color = color;
    this->default_color = color;
    this->hover_color = hover_color;
    this->click_color = click_color;
    is_colliding = false;
    clicked = false;
    this->font = font;
    this->text_color = text_color;
    this->is_in_focus = false;
    this->max_len = max_len;
    this->text_obj = Text(this->pos + Pos2D{this->width / 2, this->height / 2},
                          this->text, font, this->text_color);
    is_cursor_visible = false;
}
void InputBox::draw(Camera &camera) {
    int border_width = 2;
    if (is_in_focus)
        camera.draw_rect({rect.x - border_width, rect.y - border_width,
                          rect.w + 2 * border_width, rect.h + 2 * border_width},
                         click_color);
    camera.draw_rect(rect, color);
    int text_width = font->get_string_width(text);

    text_obj.draw(camera);
    if (is_cursor_visible)
        camera.draw_rect({rect.x + rect.w / 2 + text_width / 2 + 2,
                          rect.y + rect.h / 2 - (font->size * 2) / 3, 4,
                          (font->size * 4) / 3},
                         text_color);
}
// as long as you have called the handle_event macro previously, you should
// just be able to pass _events without defining it. Otherwise, figure it
// out yourself or call the damn macro
void InputBox::update(MouseState mouse_state, std::vector<SDL_Event> _events,
                      Keys keys) {
    is_cursor_visible = is_in_focus && (SDL_GetTicks() % 600 < 300);
    rect = {(int)pos.x, (int)pos.y, (int)width, (int)height};
    if (collide_rect(rect, {(float)mouse_state.x, (float)mouse_state.y})) {
        if (mouse_state.pressed_left()) {
            if (!clicked && is_colliding) {
                // clicked on
                is_in_focus = true;
                color = click_color;
            } else
                color = click_color;
        } else
            color = hover_color;
        is_colliding = true;

    } else {
        if (mouse_state.pressed_left()) {
            if (!clicked) {
                // clicked off
                is_in_focus = false;
            }
        }
        color = default_color;
        is_colliding = false;
    }
    clicked = mouse_state.pressed_left();

    if (is_in_focus) {
        for (auto event : _events) {
            switch (event.type) {
            case SDL_TEXTINPUT:
                // std::cout << event.text.text << std::endl;
                if (text.size() < max_len)
                    text.push_back(event.text.text[0]);
                break;
            case KEYDOWN:
                switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_BACKSPACE:
                    if (text.size() > 0)
                        text.pop_back();
                    if (keys[SDL_SCANCODE_LCTRL]) {
                        while (text.size() && text.back() != ' ') {
                            text.pop_back();
                        }
                    }
                    break;
                case SDL_SCANCODE_RETURN:
                case SDL_SCANCODE_ESCAPE:
                    is_in_focus = false;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }
    }
    this->text_obj.pos = this->pos + Pos2D{this->width / 2, this->height / 2};
    this->text_obj.text = this->text;
    this->text_obj.text_color = this->text_color;
}

Slider::Slider(Pos2D start, Pos2D end, float max_value, Color track_color,
               Color bob_color, float value) {
    this->start = start;
    this->end = end;
    this->max_value = max_value;
    this->value = value;
    this->track_color = track_color;
    this->bob_color = bob_color;
    bob_pos = start;
    is_selected = false;
}
void Slider::draw(Camera &camera) {
    camera.draw_line(start, end, track_color, 5);
    camera.draw_circle(bob_pos, 12, bob_color);
    // draw_aa_circle(screen, bob_pos, 12, bob_color);
}
void Slider::update(MouseState mouse_state) {
    bob_pos = ((end - start) * value) / max_value + start;
    if (mouse_state.pressed_left()) {
        if ((mouse_state.position() - bob_pos).norm() <= 12) {
            is_selected = true;
        }
        // std::cout << "slider!!\n";
        if (is_selected) {
            Pos2D dir = end - start;
            if (abs(dir.x) > abs(dir.y))
                value =
                    max_value * (mouse_state.x - start.x) / (end.x - start.x);
            else
                value =
                    max_value * (mouse_state.y - start.y) / (end.y - start.y);
            if (value > max_value)
                value = max_value;
            if (value < 0)
                value = 0;
        }
    } else
        is_selected = false;
}

Text::Text(Pos2D pos, std::string text, Font *font, Color text_color,
           bool centered_horizontal, bool centered_vertical,
           bool has_background, Color background_color) {
    this->pos = pos;
    this->text = text;
    this->font = font;
    this->text_color = text_color;
    this->centered_horizontal = centered_horizontal;
    this->centered_vertical = centered_vertical;
    this->has_background = has_background;
    this->background_color = background_color;
}
void Text::draw(Camera &camera) {
    int width = font->get_string_width(text);
    int ascent = font->get_ascent();
    int descent = font->get_descent();
    camera.render_text(text, pos.x - centered_horizontal * (width / 2.0),
                       pos.y + (ascent - descent) / 2.0,
                       {text_color.r / (float)255, text_color.g / (float)255,
                        text_color.b / (float)255},
                       font);
}

void _select(void *arg) {
    selector_args *s = (selector_args *)arg;
    s->selector->selected = s->selected;
    s->selector->is_dropped_down = false;
}

Selector::Selector(SDL_Rect rect, Font *font, std::vector<std::string> options,
                   Color color, Color hover_color, Color click_color) {
    this->rect = rect;
    this->font = font;
    this->options = options;
    this->default_color = color;
    this->color = color;
    this->hover_color = hover_color;
    this->click_color = click_color;
    args = std::vector<selector_args>(options.size());
    original_button =
        Button(rect, "defalult!", font, color, hover_color, click_color, NULL);
    for (int i = 0; i < options.size(); i++) {
        args[i] = {this, i};
        Button new_button = Button(
            {rect.x, rect.y + (i + 1) * rect.h, rect.w, rect.h}, options[i],
            font, color, hover_color, click_color, _select, (void *)(&args[i]));
        buttons.push_back(new_button);
    }
    this->is_dropped_down = false;
    this->selected = 0;
}
void Selector::draw(Camera &camera) {
    original_button.draw(camera);
    Pos2D arrow_center = {rect.x + rect.w - rect.h / 3.0f,
                          rect.y + rect.h / 2.0f};
    if (is_dropped_down) {
        for (int i = 0; i < options.size(); i++) {
            buttons[i].draw(camera);
        }
        camera.draw_quad(arrow_center + Pos2D{0., -4.},
                         arrow_center + Pos2D{6., 4.},
                         arrow_center + Pos2D{6., 4.},
                         arrow_center + Pos2D{-6., 4.}, {50, 50, 50, 255});
    } else {
        camera.draw_quad(arrow_center + Pos2D{6., -4.},
                         arrow_center + Pos2D{0., 4.},
                         arrow_center + Pos2D{0., 4.},
                         arrow_center + Pos2D{-6., -4.}, {50, 50, 50, 255});
    }
}

void Selector::update(MouseState mouse_state) {
    original_button.update(mouse_state);
    if (mouse_state.pressed_left() &&
        !collide_rect(
            {rect.x, rect.y, rect.w, (int)(options.size() + 1) * rect.h},
            {(float)mouse_state.x, (float)mouse_state.y})) {
        is_dropped_down = false;
    }
    if (original_button.clicked && original_button.is_colliding)
        is_dropped_down = true;

    if (is_dropped_down) {
        for (int i = 0; i < options.size(); i++) {
            buttons[i].update(mouse_state);
        }
    }
    original_button.text = options[selected];
}
