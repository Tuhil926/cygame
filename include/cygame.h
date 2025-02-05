#ifndef CYGAME_H
#define CYGAME_H
#include "SDL_scancode.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"
#include "glm/gtc/constants.hpp"
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <SDL.h>
// #include <SDL2_gfxPrimitives.h>
// #include <SDL_image.h>
#include <SDL_rect.h>
#include <SDL_stdinc.h>
#include <SDL_timer.h>
// #include <SDL_video.h>
#include <glad/glad.h>

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
#define K_shift SDL_SCANCODE_LSHIFT
#define K_ctrl SDL_SCANCODE_LCTRL
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
#define K_right SDL_SCANCODE_RIGHT

#define Mouse_left SDL_BUTTON_LMASK
#define Mouse_right SDL_BUTTON_RMASK
#define Mouse_middle SDL_BUTTON_MMASK
// add more cases when necessary please, I'm not doing the rest

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

SDL_Window *get_global_window();
SDL_GLContext get_global_gl_context();
float get_global_width();
float get_global_height();
void set_global_dimensions_to_window_width();

void make_screen(int width, int height, float gui_scale,
                 const char *title = "Opengl Game");

Keys get_keys_pressed();

void delay(int m);

void clear_screen(Color color);

void draw_screen();

float get_global_aspect_ratio();

MouseState get_mouse_state();
MouseState get_global_mouse_state();

bool collide_rect(SDL_Rect rect, Pos2D point);

bool collide_rects(SDL_Rect rect1, SDL_Rect rect2);

#define NUM_FLOATS_PER_VERTEX 11

class Shape {
  public:
    GLuint vertex_count;
    GLfloat *vertices;
    GLuint index_count;
    GLuint *indices;
    GLuint num_floats_per_vertex;
    Shape();
    GLsizeiptr get_size_bytes();
    GLsizeiptr get_indices_size_bytes();
    GLsizeiptr get_stride_bytes();
    GLsizeiptr get_color_offset();
    GLsizeiptr get_normal_offset();
    GLsizeiptr get_texture_coord_offset();
    void print_values();
};

class ShapeGenerator {
  public:
    static Shape *get_triangle();
    static Shape *get_triangle2();
    static Shape *get_rect();
    static Shape *get_cube();
    static Shape *get_circle(int num_vertices);
    static Shape *get_sphere(int num_verts_in_circle, int num_circles);
    static Shape *get_from_file(std::string filename);
};

class ShapeOnGPU {
  public:
    ShapeOnGPU();
    GLint offset_vertices;
    GLsizei num_vertices;
    GLint offset_indices;
    GLsizei num_indices;
};

class ShapeRenderer {
  public:
    GLuint vertexArrayObject;
    GLuint myBufferID;
    GLuint myElementBufferID;
    std::vector<Shape *> shapes;
    std::vector<ShapeOnGPU> shapes_on_gpu;
    int tot_offset_vertices = 0;
    int tot_offset_indices = 0;
    bool sent_already = false;
    ShapeRenderer();
    ShapeOnGPU add_shape(Shape *shape);

    void send_shapes();
    static void render_shape(ShapeOnGPU shape_gpu);
};

ShapeOnGPU get_default_rect();
ShapeOnGPU get_default_circle();

struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2 size;        // Size of glyph
    glm::ivec2 bearing;     // Offset from baseline to left/top of glyph
    long advance;           // Offset to advance to next glyph
};

class Font {
  public:
    FT_Library ft;
    FT_Face face;
    int size;
    std::map<char, Character> characters;
    Font(std::string filename, int size);
    int get_string_width(std::string &str);
    int get_ascent();
    int get_descent();
    void cleanup();
};

class Image {
  public:
    unsigned int texture_id;
    int width, height, n;
    Image();
    Image(std::string filename);
};
void set_default_vertex_shader(std::string vertex_shader_src);

void set_default_fragment_shader(std::string fragment_shader_src);

class Camera {
  public:
    glm::vec3 position;
    glm::vec3 up;
    glm::vec3 front;
    glm::vec3 right;
    float fov;
    float near_plane = 0.1;
    float far_plane = 50;
    float looking_angle = 0.0;
    MouseState prev_mouse_state;
    float movement_speed = 5;
    bool minecraft_rotation = false;
    GLuint full_transform_matrix_location;
    GLuint light_direction_uniform_location;
    GLuint rotation_matrix_uniform_location;
    GLuint camera_location_uniform_location;
    GLuint model_to_world_matrix_uniform_location;
    GLuint text_uniform_location;
    GLuint is_texture_uniform_location;
    GLuint spec_multiplier_uniform_location;
    GLuint programObject;
    Camera();
    Camera(std::string vertex_shader_src_file_name,
           std::string fragment_shader_src_file_name);
    glm::mat4 get_world_to_perspective_transform_matrix();
    void rotate_left_right(float angle);
    void rotate_up_down(float angle);
    void move_front(float distance);
    void strafe(float distance);
    void move_up(float distance);
    void track_input(Keys keys, MouseState mouse_state, float dt);
    void toggle_minecraft_rotation();
    // void draw_shape(glm::mat4 full_transform_matrix, ShapeOnGPU shape_gpu);
    void send_uniforms(glm::mat4 &full_transform_matrix,
                       glm::mat4 &model_rotation_matrix,
                       glm::mat4 &model_to_world_matrix,
                       glm::vec3 &light_direction, glm::vec3 &camera_position);
    void draw_shape(glm::mat4 model_translation_matrix,
                    glm::mat4 model_scale_matrix,
                    glm::mat4 model_rotation_matrix, ShapeOnGPU shape_gpu,
                    glm::vec3 light_direction = {1.0f, 3.0f, 2.0f},
                    bool is_2D = false);
    void send_data_to_GPU(ShapeOnGPU shape_on_gpu, GLfloat verts[]);
    void draw_2D_shape(ShapeOnGPU shape_on_gpu);
    void draw_circle(Pos2D pos, float radius, Color color);
    void render_text(std::string text, float x, float y, glm::vec3 color,
                     Font *font);
    void draw_quad(Pos2D point1, Pos2D point2, Pos2D point3, Pos2D point4,
                   Color color);
    void draw_gradient_quad(Pos2D point1, Pos2D point2, Pos2D point3,
                            Pos2D point4, Color color1, Color color2,
                            Color color3, Color color4);
    void draw_triangle(Pos2D point1, Pos2D point2, Pos2D point3, Color color);
    void draw_rect(SDL_Rect rect, Color color);
    void draw_line(Pos2D pos1, Pos2D pos2, Color color, float width);
    void draw_image_in_quad(Image &image, Pos2D point1, Pos2D point2,
                            Pos2D point3, Pos2D point4);
    void draw_image(Image &image, SDL_Rect rect);
};

class Object {
  public:
    ShapeOnGPU shape_on_gpu;
    glm::vec3 position;
    glm::mat4 rotation;
    glm::vec3 scale;
    bool has_texture = 0;
    Image image;
    Object(ShapeOnGPU shape_on_gpu);
    Object(ShapeOnGPU shape_on_gpu, Image image);
    // glm::mat4 get_model_to_world_transform_matrix();
    glm::mat4 get_model_to_world_translation_matrix();
    glm::mat4 get_model_to_world_scale_matrix();
    glm::mat4 get_model_to_world_rotation_matrix();
    void rotate_x(float angle);
    void rotate_y(float angle);
    void rotate_z(float angle);
    void draw(Camera &camera);
};

class Text {
  public:
    Pos2D pos;
    std::string text;
    Font *font;
    Color text_color;
    SDL_Rect pos_rect;
    bool centered_horizontal;
    bool centered_vertical;
    bool has_background;
    Color background_color;
    Text(Pos2D pos, std::string text, Font *font, Color text_color,
         bool centered_horizontal = true, bool centered_vertical = true,
         bool has_background = false, Color background_color = {0, 0, 0, 255});
    Text() {}
    void draw(Camera &camera);
};

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
    Font *font;
    Color text_color;
    void *arg;
    Text text_obj;
    Button() {};
    Button(SDL_Rect rect, std::string text, Font *font, Color color,
           Color hover_color, Color click_color, void (*on_click)(void *),
           void *arg = NULL, Color text_color = {0, 0, 0, 0});
    void draw(Camera &camera);

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
    Font *font;
    Color text_color;
    bool is_in_focus;
    int max_len;
    // float time_since_cursor_change;
    bool is_cursor_visible;
    Text text_obj;
    // int prev_time;
    InputBox(SDL_Rect rect, std::string text, Font *font, Color color,
             Color hover_color, Color click_color,
             Color text_color = {0, 0, 0, 0}, int max_len = 32);
    void draw(Camera &camera);
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
    void draw(Camera &camera);
    void update(MouseState mouse_state);
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
    Font *font;
    std::vector<std::string> options;
    int selected;
    std::vector<Button> buttons;
    std::vector<selector_args> args;
    Button original_button;
    bool is_dropped_down;
    Selector(SDL_Rect rect, Font *font, std::vector<std::string> options,
             Color color, Color hover_color, Color click_color);
    void draw(Camera &camera);
    void update(MouseState mouse_state);
};

#endif
