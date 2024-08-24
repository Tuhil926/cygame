#ifndef CYGAME_H
#define CYGAME_H
#include <iostream>
#include <vector>

#include "headers/SDL.h"
#include "headers/SDL2_gfxPrimitives.h"
#include "headers/SDL_image.h"
#include "headers/SDL_rect.h"
#include "headers/SDL_timer.h"
#include "headers/SDL_ttf.h"
#define QUIT SDL_QUIT
#define KEYDOWN SDL_KEYDOWN
#define KEYUP SDL_KEYUP
#define KEYMAPCHANGED SDL_KEYMAPCHANGED
#define handle_events                                                          \
    SDL_Event _event;                                                          \
    std::vector<SDL_Event> _events;                                            \
    for (; SDL_PollEvent(&_event); _events.push_back(_event))                  \
        switch (_event.type)
#define handle_keycode switch (_event.key.keysym.scancode)
#define DEFAULT_FONT "fonts/PixelOperator8.ttf"

#define K_a SDL_SCANCODE_A
#define K_b SDL_SCANCODE_B
#define K_c SDL_SCANCODE_C
#define K_d SDL_SCANCODE_D
#define K_e SDL_SCANCODE_E
#define K_f SDL_SCANCODE_F
#define K_g SDL_SCANCODE_G
#define K_h SDL_SCANCODE_H
#define K_i SDL_SCANCODE_I
#define K_j SDL_SCANCODE_J
#define K_k SDL_SCANCODE_K
#define K_l SDL_SCANCODE_L
#define K_m SDL_SCANCODE_M
#define K_n SDL_SCANCODE_N
#define K_o SDL_SCANCODE_O
#define K_p SDL_SCANCODE_P
#define K_q SDL_SCANCODE_Q
#define K_r SDL_SCANCODE_R
#define K_s SDL_SCANCODE_S
#define K_t SDL_SCANCODE_T
#define K_u SDL_SCANCODE_U
#define K_v SDL_SCANCODE_V
#define K_w SDL_SCANCODE_W
#define K_x SDL_SCANCODE_X
#define K_y SDL_SCANCODE_Y
#define K_z SDL_SCANCODE_Z
#define K_space SDL_SCANCODE_SPACE
#define K_0 SDL_SCANCODE_0
#define K_1 SDL_SCANCODE_1
#define K_2 SDL_SCANCODE_2
#define K_3 SDL_SCANCODE_3
#define K_4 SDL_SCANCODE_4
#define K_5 SDL_SCANCODE_5
#define K_6 SDL_SCANCODE_6
#define K_7 SDL_SCANCODE_7
#define K_8 SDL_SCANCODE_8
#define K_9 SDL_SCANCODE_9
#define K_up SDL_SCANCODE_UP
#define K_down SDL_SCANCODE_DOWN
#define K_left SDL_SCANCODE_LEFT
#define K_ring SDL_SCANCODE_RIGHT

#define Mouse_left SDL_BUTTON_LMASK
#define Mouse_right SDL_BUTTON_RMASK
#define Mouse_middle SDL_BUTTON_MMASK
// add more cases when necessary please, I'm not doing the rest

typedef SDL_Renderer *CYScreen;
typedef const Uint8 *Keys;

struct Color {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
    operator SDL_Color() { return {r, g, b, a}; }
    operator Uint32() { return (a << 24) | (b << 16) | (g << 8) | r; }
} typedef Color;

struct Pos2D {
    float x;
    float y;
    Pos2D operator+(const Pos2D &other) { return {other.x + x, other.y + y}; }
    Pos2D operator-(const Pos2D &other) { return {x - other.x, y - other.y}; }
    Pos2D operator/(const float num) { return {x / num, y / num}; }
    Pos2D operator*(const float num) { return {x * num, y * num}; }
    Pos2D operator*(const Pos2D &other) { return {x * other.x, y * other.y}; }
    float norm() { return sqrt(x * x + y * y); }
} typedef Pos2D;

struct MouseState {
    int x;
    int y;
    Uint32 buttons;
    bool pressed_left() { return buttons & Mouse_left; }
    bool pressed_right() { return buttons & Mouse_right; }
    bool pressed_middle() { return buttons & Mouse_middle; }
    Pos2D position() { return {(float)x, (float)y}; }
} typedef MouseState;

int cygame_init();

CYScreen make_screen(int width, int height, float gui_scale = 1,
                     const char *title = "Game");

void draw_rect(SDL_Rect rect, CYScreen screen, Color color);

void fill_rect(SDL_Rect rect, CYScreen screen, Color color);

void draw_line(CYScreen screen, Pos2D pos1, Pos2D pos2, Color color, int width);

void draw_polygon(CYScreen screen, std::vector<Pos2D> vertices, Color color);

void draw_gradient_polygon(CYScreen screen, std::vector<SDL_Vertex> vertices);

Keys get_keys_pressed();

void delay(int m);

void clear_screen(CYScreen screen);

void draw_screen(CYScreen screen);

MouseState get_mouse_state();
MouseState get_global_mouse_state();

bool collide_rect(SDL_Rect rect, Pos2D point);

bool collide_rects(SDL_Rect rect1, SDL_Rect rect2);

int draw_centered_text(CYScreen screen, TTF_Font *font, std::string text,
                       Pos2D pos_center, Color color);

void draw_aa_circle(CYScreen screen, Pos2D pos, int radius, Color color);

// a button which takes a callback functions with a void* argument
class Button {
  public:
    Pos2D pos;
    float width;
    float height;
    std::string text;
    Color color;
    Color default_color;
    Color hover_color;
    Color click_color;
    bool is_colliding;
    bool clicked;
    SDL_Rect rect;
    void (*on_click)(void *);
    TTF_Font *font;
    Color text_color;
    void *arg;
    Button();
    Button(SDL_Rect rect, std::string text, int font_size, Color color,
           Color hover_color, Color click_color, void (*on_click)(void *),
           void *arg = NULL, Color text_color = {0, 0, 0, 0});
    void draw(CYScreen screen);

    void update(MouseState mouse_state);
};

class InputBox {
  public:
    Pos2D pos;
    float width;
    float height;
    std::string text;
    Color color;
    Color default_color;
    Color hover_color;
    Color click_color;
    bool is_colliding;
    bool clicked;
    SDL_Rect rect;
    // void (*on_click)(void*);
    TTF_Font *font;
    Color text_color;
    bool is_in_focus;
    int max_len;
    int font_size;
    // float time_since_cursor_change;
    bool is_cursor_visible;
    // int prev_time;
    InputBox(SDL_Rect rect, std::string text, int font_size, Color color,
             Color hover_color, Color click_color,
             Color text_color = {0, 0, 0, 0}, int max_len = 32);
    void draw(CYScreen screen);
    // as long as you have called the handle_event macro previously, you
    // should just be able to pass _events without defining it. Otherwise,
    // figure it out yourself. Or just call the damn macro, it's not that hard.
    // Why are you not using it anyways? How do you quit your application if
    // you're not checking for the quit event? Why am I ranting while writing
    // documentation?
    void update(MouseState mouse_state, std::vector<SDL_Event> _eventsk,
                Keys keys);
};

class Slider {
  public:
    Pos2D start;
    Pos2D end;
    float max_value;
    float value;
    Color track_color;
    Color bob_color;
    Pos2D bob_pos;
    bool is_selected;
    Slider(Pos2D start, Pos2D end, float max_value, Color track_color,
           Color bob_color, float value = 0);
    void draw(CYScreen screen);
    void update(MouseState mouse_state);
};

// use this when you want to render text, but dont't want the extra cost of
// re-rendering the text from the string every frame. Use this only if you're
// not changing the text very often, or better, not at all.(I wrote this, and
// immediately broke this rule. Ig go ahead and use it for all text, because
// it's the same performance as using my draw_centered_text function, but more
// convenient.)
class StaticText {
  public:
    Pos2D pos;
    std::string text;
    Color color;
    TTF_Font *font;
    int font_size;
    Color text_color;
    SDL_Surface *text_surface;
    SDL_Texture *text_texture;
    CYScreen screen;
    SDL_Rect pos_rect;
    StaticText(Pos2D pos, std::string text, int font_size, Color text_color,
               CYScreen screen, bool centered_horizontal = true,
               bool centered_vertical = true, bool has_background = false,
               Color background_color = {200, 200, 200, 255});
    ~StaticText();
    void set_text(std::string new_text);
    void change_font_size(int font_size);
    // re-initialises the rendered font. Call this when you change the text,
    // color or position
    void re_render();
    void draw();
};
class Selector;
struct selector_args {
    Selector *selector;
    int selected;
} typedef selector_args;

void _select(void *arg);
class Selector {
  public:
    SDL_Rect rect;
    Color color;
    Color default_color;

    Color hover_color;
    Color click_color;
    TTF_Font *font;
    std::vector<std::string> options;
    int selected;
    std::vector<Button> buttons;
    std::vector<selector_args> args;
    Button original_button;
    bool is_dropped_down;
    Selector(SDL_Rect rect, int font_size, std::vector<std::string> options,
             Color color, Color hover_color, Color click_color);
    void draw(CYScreen screen);
    void update(MouseState mouse_state);
};

#endif
