#include "cygame.h"
#include "headers/SDL_stdinc.h"

#include <iostream>
#include <ostream>
#include <vector>
// initialises SDL2 and SDL2_TTF
int cygame_init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }

    TTF_Init();
    std::cout << "Welcome to the cygame community (estimated size: 1 person)\n";
    return 0;
}
SDL_Window *global_window;
float global_width, global_height;

/// @brief returns an SDL_Renderer in a window onto which all your subsequent
/// graphics can be rendered to
/// @param width
/// @param height
/// @param gui_scale the current gui scale of your laptop. 1 by default, but if
/// it's set to 1.5 for example, then the final screen will be scaled down
/// by 1.5
/// @return
CYScreen make_screen(int width, int height, float gui_scale,
                     const char *title) {
    SDL_Window *win = SDL_CreateWindow(
        title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        (int)((float)width / gui_scale), (int)((float)height / gui_scale),
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    global_window = win;

    // triggers the program that controls
    // your graphics hardware and sets flags
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;

    // creates a renderer to render our images
    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, render_flags);
    SDL_RenderSetLogicalSize(rend, width, height);
    global_width = width;
    global_height = height;
    return rend;
}

// draws a rectangle
void draw_rect(SDL_Rect rect, CYScreen screen, Color color) {
    SDL_SetRenderDrawColor(screen, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRect(screen, &rect);
    SDL_SetRenderDrawColor(screen, 0, 0, 0, 255);
}

// draws a filled rectangle
void fill_rect(SDL_Rect rect, CYScreen screen, Color color) {
    SDL_SetRenderDrawColor(screen, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(screen, &rect);
    SDL_SetRenderDrawColor(screen, 0, 0, 0, 255);
}

// draws a line. if width is provided, it basically draws a rectangle which goes
// from pos1 to pos2 and has twice the width.
void draw_line(CYScreen screen, Pos2D pos1, Pos2D pos2, Color color,
               int width) {
    if (!width)
        return;
    SDL_SetRenderDrawColor(screen, color.r, color.g, color.b, color.a);
    if (width == 1) {
        SDL_RenderDrawLineF(screen, pos1.x, pos1.y, pos2.x, pos2.y);
    } else {
        Pos2D dir = (pos2 - pos1);
        Pos2D normal = dir * width / dir.norm();
        Pos2D perpendicular = {normal.y, -normal.x};
        std::vector<Pos2D> vertices = {
            pos1 + perpendicular, pos1 - perpendicular, pos2 - perpendicular,
            pos2 + perpendicular};
        std::vector<SDL_Vertex> v;
        for (auto vertex : vertices) {
            v.push_back({{vertex.x, vertex.y},
                         {color.r, color.g, color.b, color.a},
                         {0.f, 0.f}});
        }
        // std::cout << v[3].position.x << ' ' << v[3].position.y << std::endl;
        int indices[6] = {0, 1, 2, 2, 3, 0};
        aalineColor(screen, vertices[0].x, vertices[0].y, vertices[1].x,
                    vertices[1].y, color);
        aalineColor(screen, vertices[1].x, vertices[1].y, vertices[2].x,
                    vertices[2].y, color);
        aalineColor(screen, vertices[2].x, vertices[2].y, vertices[3].x,
                    vertices[3].y, color);
        aalineColor(screen, vertices[3].x, vertices[3].y, vertices[0].x,
                    vertices[0].y, color);

        SDL_RenderGeometry(screen, NULL, v.data(), 4, indices, 6);
    }
    SDL_SetRenderDrawColor(screen, 0, 0, 0, 255);
}

// draws a polygon. It draws triangles with the first point being the starting
// point and taking 2 of the remaining points at a time.
void draw_polygon(CYScreen screen, std::vector<Pos2D> vertices, Color color) {
    std::vector<SDL_Vertex> v;
    for (auto vertex : vertices) {
        v.push_back({{vertex.x, vertex.y},
                     {color.r, color.g, color.b, color.a},
                     {0.f, 0.f}});
    }
    std::vector<int> indices;
    for (int i = 1; i < vertices.size() - 1; i++) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i + 1);
    }
    SDL_RenderGeometry(screen, NULL, v.data(), v.size(), indices.data(),
                       indices.size());
}
// draws a polygon. It draws triangles with the first point being the starting
// point and taking 2 of the remaining points at a time. With this one, you must
// supply a list of SDL_Vertex s', where each point will have a color associated
// with it.
void draw_gradient_polygon(CYScreen screen, std::vector<SDL_Vertex> vertices) {
    std::vector<int> indices;
    for (int i = 1; i < vertices.size() - 1; i++) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i + 1);
    }
    SDL_RenderGeometry(screen, NULL, vertices.data(), vertices.size(),
                       indices.data(), indices.size());
}

// gets a list of keys that are currently held down (Keys is Uint8 *).
// You can use this instead of keydown events to detect keys being
// pressed, because it's faster.
Keys get_keys_pressed() { return SDL_GetKeyboardState(0); }

// just calls SDL_Delay, which sleeps for m milliseconds. Idk why I wrote this
// function, I guess it just makes it convenient to type out and consistent with
// the rest of stuff by removing the SDL prefix. You don't have to use this tho
void delay(int m) { SDL_Delay(m); }

// clears the screen. You can change draw color to whatever you want by editing
// this function.
void clear_screen(CYScreen screen) {
    SDL_SetRenderDrawColor(screen, 60, 60, 60, 255);
    SDL_RenderClear(screen);
}

// Fills the screen with a color
void fill_screen(CYScreen screen, Color color) {
    SDL_SetRenderDrawColor(screen, color.r, color.g, color.b, color.a);
    SDL_RenderClear(screen);
}

// shows whatever you have drawn on to the screen so far.
void draw_screen(CYScreen screen) { SDL_RenderPresent(screen); }

// returns the current state of the mouse, which are it's x and y coordinates
// with respect to the window, as well as information about what mouse buttons
// are being pressed. I fixed a bug in SDL2 where the SDL_GetMouseState doesn't
// take into account that the screen has been scaled. I just re calculate the x
// and y values and return them.
MouseState get_mouse_state() {
    int x, y;
    Uint32 buttons = SDL_GetMouseState(&x, &y);

    int w, h;
    SDL_GetWindowSize(global_window, &w, &h);
    float fx = x, fy = y, fw = w, fh = h;
    if (fw / fh < global_width / global_height) {
        float scaling_factor = fw / global_width;
        float y_offset = (h - (global_height * scaling_factor)) / 2.;
        x = fx / scaling_factor;
        y = (fy - y_offset) / scaling_factor;
    } else {
        float scaling_factor = fh / global_height;
        float x_offset = (w - (global_width * scaling_factor)) / 2.;
        x = (fx - x_offset) / scaling_factor;
        y = fy / scaling_factor;
    }
    return {x, y, buttons};
}

// same as get_mouse_state, but gets the global position instead.
MouseState get_global_mouse_state() {
    int x, y;
    Uint32 buttons = SDL_GetGlobalMouseState(&x, &y);
    return {x, y, buttons};
}

// checks if a rect is colliding with a point and returns true if so.
bool collide_rect(SDL_Rect rect, Pos2D point) {
    return (point.x >= rect.x) && (point.y >= rect.y) &&
           (point.x <= rect.x + rect.w) && (point.y <= rect.y + rect.h);
}

// checks if a rect is colliding with another rect and returns true if so.
bool collide_rects(SDL_Rect rect1, SDL_Rect rect2) {
    return collide_rect(rect1, {(float)rect2.x, (float)rect2.y}) ||
           collide_rect(rect1, {(float)(rect2.x + rect2.w), (float)rect2.y}) ||
           collide_rect(rect1, {(float)rect2.x, (float)(rect2.y + rect2.h)}) ||
           collide_rect(
               rect1, {(float)(rect2.x + rect2.w), (float)(rect2.y + rect2.h)});
}

// draws text centered around pos_center
int draw_centered_text(CYScreen screen, TTF_Font *font, std::string text,
                       Pos2D pos_center, Color color) {
    SDL_Surface *text_surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture *text_texture =
        SDL_CreateTextureFromSurface(screen, text_surface);
    SDL_Rect pos_rect = {(int)(pos_center.x - text_surface->w / 2),
                         (int)(pos_center.y - text_surface->h / 2),
                         text_surface->w, text_surface->h};
    SDL_RenderCopy(screen, text_texture, NULL, &pos_rect);
    int ret = text_surface->w;
    SDL_FreeSurface(text_surface);
    SDL_DestroyTexture(text_texture);
    return ret;
}

// draws an anti-aliased filled circle. For some reason such a function does not
// exist in SDL2_gfx, so I make a workaround by just drawing an (unfilled)
// aacircle and then drawing a non-anti-aliased filled circle. It's not perfect,
// but it's better than no anti aliasing.
void draw_aa_circle(CYScreen screen, Pos2D pos, int radius, Color color) {
    aacircleColor(screen, pos.x, pos.y, radius, color);
    filledCircleColor(screen, pos.x, pos.y, radius, color);
}

Button::Button(SDL_Rect rect, std::string text, int font_size, Color color,
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
    font = TTF_OpenFont(DEFAULT_FONT, font_size);
    this->text_color = text_color;
}
void Button::draw(CYScreen screen) {
    fill_rect(rect, screen, color);
    if (text.size())
        draw_centered_text(screen, font, text,
                           pos + Pos2D{width / 2, height / 2}, text_color);
}
Button::Button() {}
void Button::update(MouseState mouse_state) {
    rect = {(int)pos.x, (int)pos.y, (int)width, (int)height};
    if (collide_rect(rect, {(float)mouse_state.x, (float)mouse_state.y})) {
        if (mouse_state.pressed_left()) {
            if (!clicked && is_colliding) {
                std::cout << "ff\n";
                if (on_click) {
                    std::cout << "nice but wtf\n";
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
}

InputBox::InputBox(SDL_Rect rect, std::string text, int font_size, Color color,
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
    font = TTF_OpenFont(DEFAULT_FONT, font_size);
    this->text_color = text_color;
    this->is_in_focus = false;
    this->max_len = max_len;
    this->font_size = font_size;
    is_cursor_visible = false;
}
void InputBox::draw(CYScreen screen) {
    int border_width = 2;
    if (is_in_focus)
        fill_rect({rect.x - border_width, rect.y - border_width,
                   rect.w + 2 * border_width, rect.h + 2 * border_width},
                  screen, click_color);
    fill_rect(rect, screen, color);
    int text_width = 0;
    if (text.size())
        text_width = draw_centered_text(
            screen, font, text, pos + Pos2D{width / 2, height / 2}, text_color);
    if (is_cursor_visible)
        fill_rect({rect.x + rect.w / 2 + text_width / 2 + 2,
                   rect.y + rect.h / 2 - (font_size * 2) / 3, 4,
                   (font_size * 4) / 3},
                  screen, text_color);
}
// as long as you have called the handle_event macro previously, you should
// just be able to pass _events without defining it. Otherwise, figure it
// out yourself or call the damn macro
void InputBox::update(MouseState mouse_state, std::vector<SDL_Event> _events,
                      Keys keys) {
    is_cursor_visible = is_in_focus && (SDL_GetTicks() % 1000 < 500);
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
                }
                break;
            default:
                break;
            }
        }
    }
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
void Slider::draw(CYScreen screen) {
    draw_line(screen, start, end, track_color, 5);
    draw_aa_circle(screen, bob_pos, 12, bob_color);
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

StaticText::StaticText(Pos2D pos, std::string text, int font_size,
                       Color text_color, CYScreen screen,
                       bool centered_horizontal, bool centered_vertical,
                       bool has_background, Color background_color) {
    this->pos = pos;
    this->text = text;
    this->color = background_color;
    this->text_color = text_color;
    this->font_size = font_size;
    this->screen = screen;
    font = TTF_OpenFont(DEFAULT_FONT, font_size);
    text_surface = TTF_RenderText_Solid(font, text.c_str(), color);
    text_texture = SDL_CreateTextureFromSurface(screen, text_surface);
    pos_rect = {(int)(pos.x - (text_surface->w / 2) * centered_horizontal),
                (int)(pos.y - (text_surface->h / 2) * centered_vertical),
                text_surface->w, text_surface->h};
}
StaticText::~StaticText() {
    SDL_FreeSurface(text_surface);
    SDL_DestroyTexture(text_texture);
}

void StaticText::change_font_size(int font_size) {
    this->font_size = font_size;
    font = TTF_OpenFont(DEFAULT_FONT, font_size);
}

void StaticText::set_text(std::string new_text) {
    text = new_text;
    re_render();
}
// re-initialises the rendered font. Call this when you change the text,
// color or position
void StaticText::re_render() {
    SDL_FreeSurface(text_surface);
    SDL_DestroyTexture(text_texture);
    text_surface = TTF_RenderText_Solid(font, text.c_str(), color);
    text_texture = SDL_CreateTextureFromSurface(screen, text_surface);
    pos_rect = {(int)(pos.x - text_surface->w / 2),
                (int)(pos.y - text_surface->h / 2), text_surface->w,
                text_surface->h};
}
void StaticText::draw() {
    SDL_RenderCopy(screen, text_texture, NULL, &pos_rect);
}

void _select(void *arg) {
    selector_args *s = (selector_args *)arg;
    s->selector->selected = s->selected;
    s->selector->is_dropped_down = false;
}

Selector::Selector(SDL_Rect rect, int font_size,
                   std::vector<std::string> options, Color color,
                   Color hover_color, Color click_color) {
    this->rect = rect;
    this->font = TTF_OpenFont(DEFAULT_FONT, font_size);
    this->options = options;
    this->default_color = color;
    this->color = color;
    this->hover_color = hover_color;
    this->click_color = click_color;
    args = std::vector<selector_args>(options.size());
    original_button = Button(rect, "defalult!", font_size, color, hover_color,
                             click_color, NULL);
    for (int i = 0; i < options.size(); i++) {
        args[i] = {this, i};
        Button new_button =
            Button({rect.x, rect.y + (i + 1) * rect.h, rect.w, rect.h},
                   options[i], font_size, color, hover_color, click_color,
                   _select, (void *)(&args[i]));
        buttons.push_back(new_button);
    }
    this->is_dropped_down = false;
    this->selected = 0;
}
void Selector::draw(CYScreen screen) {
    original_button.draw(screen);
    Pos2D arrow_center = {rect.x + rect.w - (float)rect.h / 3,
                          rect.y + (float)rect.h / 2};
    if (is_dropped_down) {
        for (int i = 0; i < options.size(); i++) {
            buttons[i].draw(screen);
        }
        draw_polygon(screen,
                     {arrow_center + Pos2D{0., -4.},
                      arrow_center + Pos2D{6., 4.},
                      arrow_center + Pos2D{-6., 4.}},
                     {50, 50, 50, 255});
    } else {
        draw_polygon(screen,
                     {arrow_center + Pos2D{0., 4.},
                      arrow_center + Pos2D{6., -4.},
                      arrow_center + Pos2D{-6., -4.}},
                     {50, 50, 50, 255});
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
