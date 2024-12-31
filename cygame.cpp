#include "cygame.h"
#include "SDL_events.h"
#include "SDL_mouse.h"
#include "SDL_video.h"
#include "glad/glad.h"
#include "glm/common.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"
#include "glm/geometric.hpp"
#include "glm/gtc/constants.hpp"
#include <fstream>
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
/// @return CYScreen
CYScreen make_screen(int width, int height, float gui_scale,
                     const char *title) {
    SDL_Window *win =
        SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         (int)(width / gui_scale), (int)(height / gui_scale),
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

CYGLScreen make_opengl_screen(int width, int height, float gui_scale,
                              const char *title) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_Window *win = SDL_CreateWindow(
        title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        (int)(width / gui_scale), (int)(height / gui_scale),
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    global_window = win;

    global_width = width;
    global_height = height;
    CYGLScreen rend = SDL_GL_CreateContext(win);

    int err = gladLoadGLLoader(SDL_GL_GetProcAddress);
    if (!err) {
        printf("error initializing glad: %d\n", err);
        exit(1);
    }

    glViewport(0, 0, width, height);

    std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN_DESKTOP);

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

void clear_opengl_screen(Color color) {
    int w, h;
    SDL_GetWindowSize(global_window, &w, &h);
    global_width = w;
    global_height = h;
    glViewport(0, 0, global_width, global_height);
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

float get_global_aspect_ratio() { return global_width / (float)global_height; }

// Fills the screen with a color
void fill_screen(CYScreen screen, Color color) {
    SDL_SetRenderDrawColor(screen, color.r, color.g, color.b, color.a);
    SDL_RenderClear(screen);
}

// shows whatever you have drawn on to the screen so far.
void draw_screen(CYScreen screen) { SDL_RenderPresent(screen); }

void draw_opengl_screen() { SDL_GL_SwapWindow(global_window); }

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
    SDL_Rect pos_rect = {(int)(pos_center.x - (float)text_surface->w / 2),
                         (int)(pos_center.y - (float)text_surface->h / 2),
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

Shape::Shape() {
    vertex_count = 0;
    index_count = 0;
    vertices = NULL;
    indices = NULL;
    num_floats_per_vertex = 0;
}

GLsizeiptr Shape::get_size_bytes() {
    return vertex_count * num_floats_per_vertex * sizeof(GLfloat);
}
GLsizeiptr Shape::get_indices_size_bytes() {
    return index_count * sizeof(GLushort);
}
GLsizeiptr Shape::get_stride_bytes() {
    return num_floats_per_vertex * sizeof(GLfloat);
}
GLsizeiptr Shape::get_color_offset() { return (3) * sizeof(GLfloat); }
GLsizeiptr Shape::get_normal_offset() { return (6) * sizeof(GLfloat); }

const int NUM_FLOATS_PER_VERTEX = 9;

Shape *ShapeGenerator::get_triangle() {
    Shape *ret = new Shape();
    ret->vertex_count = 3;
    ret->index_count = 3;
    ret->num_floats_per_vertex = 6;
    GLfloat verts[] = {
        0.0f,  1.0f,  -1.0f, // vertex1
        1.0f,  0.0f,  0.0f,  // color1
        1.0f,  -1.0f, 0.0f,  // vertex2
        0.0f,  1.0f,  0.0f,  // color2
        -1.0f, -1.0f, 0.0f,  // vertex3
        0.0f,  0.0f,  1.0f   // color3
    };
    GLushort inds[] = {0, 1, 2};
    ret->vertices = (GLfloat *)malloc(ret->get_size_bytes());
    ret->indices = (GLushort *)malloc(ret->get_indices_size_bytes());
    memcpy(ret->vertices, verts, ret->get_size_bytes());
    memcpy(ret->indices, inds, ret->get_indices_size_bytes());
    return ret;
}
Shape *ShapeGenerator::get_triangle2() {
    Shape *ret = new Shape();
    ret->vertex_count = 3;
    ret->index_count = 3;
    ret->num_floats_per_vertex = 6;
    GLfloat verts[] = {
        -1.0f, 1.0f,  0.0f,  // vertex1
        1.0f,  0.0f,  0.0f,  // color1
        1.0f,  1.0f,  0.0f,  // vertex2
        0.0f,  1.0f,  0.0f,  // color2
        0.0f,  -1.0f, -1.0f, // vertex3
        0.0f,  0.0f,  1.0f   // color3
    };
    GLushort inds[] = {0, 1, 2};
    ret->vertices = (GLfloat *)malloc(ret->get_size_bytes());
    ret->indices = (GLushort *)malloc(ret->get_indices_size_bytes());
    memcpy(ret->vertices, verts, ret->get_size_bytes());
    memcpy(ret->indices, inds, ret->get_indices_size_bytes());
    return ret;
}

Shape *ShapeGenerator::get_cube() {
    Shape *ret = new Shape();
    ret->vertex_count = 24;
    ret->index_count = 36;
    ret->num_floats_per_vertex = NUM_FLOATS_PER_VERTEX;
    GLfloat verts[] = {
        1.0,       1.0,  1.0,  // top face
        /**/ 1.0,  1.0,  0.0,  //
        /**/ 0.0,  0.0,  1.0,  //
        -1.0,      1.0,  1.0,  //
        /**/ 1.0,  0.0,  1.0,  //
        /**/ 0.0,  0.0,  1.0,  //
        -1.0,      -1.0, 1.0,  //
        /**/ 0.0,  0.0,  1.0,  //
        /**/ 0.0,  0.0,  1.0,  //
        1.0,       -1.0, 1.0,  //
        /**/ 1.0,  0.0,  0.0,  //
        /**/ 0.0,  0.0,  1.0,  //
        1.0,       1.0,  -1.0, // bottom face
        /**/ 0.0,  1.0,  1.0,  //
        /**/ 0.0,  0.0,  -1.0, //
        -1.0,      1.0,  -1.0, //
        /**/ 0.0,  1.0,  0.0,  //
        /**/ 0.0,  0.0,  -1.0, //
        -1.0,      -1.0, -1.0, //
        /**/ 0.0,  1.0,  0.5,  //
        /**/ 0.0,  0.0,  -1.0, //
        1.0,       -1.0, -1.0, //
        /**/ 0.5,  1.0,  0.5,  //
        /**/ 0.0,  0.0,  -1.0, //
        1.0,       1.0,  1.0,  // right face
        /**/ 1.0,  0.0,  0.5,  //
        /**/ 1.0,  0.0,  0.0,  //
        1.0,       -1.0, 1.0,  //
        /**/ 1.0,  0.0,  1.0,  //
        /**/ 1.0,  0.0,  0.0,  //
        1.0,       -1.0, -1.0, //
        /**/ 0.0,  0.0,  1.0,  //
        /**/ 1.0,  0.0,  0.0,  //
        1.0,       1.0,  -1.0, //
        /**/ 0.0,  0.5,  1.0,  //
        /**/ 1.0,  0.0,  0.0,  //
        -1.0,      1.0,  1.0,  // left face
        /**/ 1.0,  0.5,  1.0,  //
        /**/ -1.0, 0.0,  0.0,  //
        -1.0,      -1.0, 1.0,  //
        /**/ 1.0,  0.5,  0.5,  //
        /**/ -1.0, 0.0,  0.0,  //
        -1.0,      -1.0, -1.0, //
        /**/ 0.5,  0.5,  0.5,  //
        /**/ -1.0, 0.0,  0.0,  //
        -1.0,      1.0,  -1.0, //
        /**/ 0.5,  1.0,  0.5,  //
        /**/ -1.0, 0.0,  0.0,  //
        1.0,       1.0,  1.0,  // front face
        /**/ 0.5,  1.0,  1.0,  //
        /**/ 0.0,  1.0,  0.0,  //
        -1.0,      1.0,  1.0,  //
        /**/ 0.0,  0.0,  1.0,  //
        /**/ 0.0,  1.0,  0.0,  //
        -1.0,      1.0,  -1.0, //
        /**/ 0.0,  1.0,  0.0,  //
        /**/ 0.0,  1.0,  0.0,  //
        1.0,       1.0,  -1.0, //
        /**/ 1.0,  1.0,  0.0,  //
        /**/ 0.0,  1.0,  0.0,  //
        1.0,       -1.0, 1.0,  // back face
        /**/ 1.0,  0.0,  0.0,  //
        /**/ 0.0,  -1.0, 0.0,  //
        -1.0,      -1.0, 1.0,  //
        /**/ 1.0,  0.0,  1.0,  //
        /**/ 0.0,  -1.0, 0.0,  //
        -1.0,      -1.0, -1.0, //
        /**/ 1.0,  1.0,  0.0,  //
        /**/ 0.0,  -1.0, 0.0,  //
        1.0,       -1.0, -1.0, //
        /**/ 0.0,  1.0,  0.0,  //
        /**/ 0.0,  -1.0, 0.0,  //
    };
    GLushort inds[] = {
        0,  1,  2,  2,  3,  0,  //
        4,  6,  5,  6,  4,  7,  //
        8,  9,  10, 10, 11, 8,  //
        12, 14, 13, 14, 12, 15, //
        16, 18, 17, 18, 16, 19, //
        20, 21, 22, 22, 23, 20  //
    };
    ret->vertices = (GLfloat *)malloc(ret->get_size_bytes());
    ret->indices = (GLushort *)malloc(ret->get_indices_size_bytes());
    memcpy(ret->vertices, verts, ret->get_size_bytes());
    memcpy(ret->indices, inds, ret->get_indices_size_bytes());
    return ret;
}

Shape *ShapeGenerator::get_sphere(int num_verts_in_circle, int num_circles) {
    Shape *ret = new Shape();
    ret->vertex_count = 2 + num_verts_in_circle * num_circles;
    ret->index_count = (num_circles - 1) * num_verts_in_circle * 6 +
                       2 * num_verts_in_circle * 3;
    ret->num_floats_per_vertex = NUM_FLOATS_PER_VERTEX;
    // GLfloat verts[] = {
    //     -1.0f, 1.0f,  0.0f,  // vertex1
    //     1.0f,  0.0f,  0.0f,  // color1
    //     1.0f,  1.0f,  0.0f,  // vertex2
    //     0.0f,  1.0f,  0.0f,  // color2
    //     0.0f,  -1.0f, -1.0f, // vertex3
    //     0.0f,  0.0f,  1.0f   // color3
    // };
    // GLushort inds[] = {0, 1, 2};
    std::vector<GLfloat> verts;
    std::vector<GLushort> inds;
    float angle = -glm::half_pi<float>();
    for (int i = 0; i < num_circles; i++) {
        glm::vec4 vert(1.0f, 0.0f, 0.0f, 0.0f);
        angle += glm::pi<float>() / (float)(num_circles + 1);
        auto rotate_mat = glm::rotate(glm::mat4(1), angle, {0.0f, 0.0f, 1.0f});
        vert = rotate_mat * vert;
        float y_angle = 0.0f;
        for (int j = 0; j < num_verts_in_circle; j++) {
            auto rotate_mat_y =
                glm::rotate(glm::mat4(1), y_angle, {0.0f, 1.0f, 0.0f});
            auto new_vert = rotate_mat_y * vert;
            // position
            verts.push_back(new_vert.x);
            verts.push_back(new_vert.y);
            verts.push_back(new_vert.z);

            // color
            verts.push_back(1.0);
            verts.push_back(1.0);
            verts.push_back(1.0);

            // normal
            verts.push_back(new_vert.x);
            verts.push_back(new_vert.y);
            verts.push_back(new_vert.z);

            y_angle += glm::two_pi<float>() / (float)num_verts_in_circle;
        }
    }
    // top vertex
    verts.push_back(0.0);
    verts.push_back(1.0);
    verts.push_back(0.0);

    verts.push_back(1.0);
    verts.push_back(1.0);
    verts.push_back(1.0);

    verts.push_back(0.0);
    verts.push_back(1.0);
    verts.push_back(0.0);

    // bottom vertex
    verts.push_back(0.0);
    verts.push_back(-1.0);
    verts.push_back(0.0);

    verts.push_back(1.0);
    verts.push_back(1.0);
    verts.push_back(1.0);

    verts.push_back(0.0);
    verts.push_back(-1.0);
    verts.push_back(0.0);

    for (int circle_ind = 0; circle_ind < num_circles - 1; circle_ind++) {
        for (int vert_ind = 0; vert_ind < num_verts_in_circle; vert_ind++) {
            int actual_vert_ind1 = circle_ind * num_verts_in_circle + vert_ind;
            int actual_vert_ind2 = circle_ind * num_verts_in_circle +
                                   (vert_ind + 1) % num_verts_in_circle;
            int actual_vert_ind3 = (circle_ind + 1) * num_verts_in_circle +
                                   (vert_ind + 1) % num_verts_in_circle;
            int actual_vert_ind4 =
                (circle_ind + 1) * num_verts_in_circle + vert_ind;
            inds.push_back(actual_vert_ind1);
            inds.push_back(actual_vert_ind2);
            inds.push_back(actual_vert_ind3);
            inds.push_back(actual_vert_ind1);
            inds.push_back(actual_vert_ind3);
            inds.push_back(actual_vert_ind4);
        }
    }
    for (int vert_ind = 0; vert_ind < num_verts_in_circle; vert_ind++) {
        int top_vert_ind = num_verts_in_circle * num_circles;
        int actual_vert_ind_top1 =
            (num_circles - 1) * num_verts_in_circle + vert_ind;
        int actual_vert_ind_top2 = (num_circles - 1) * num_verts_in_circle +
                                   (vert_ind + 1) % num_verts_in_circle;
        inds.push_back(actual_vert_ind_top1);
        inds.push_back(actual_vert_ind_top2);
        inds.push_back(top_vert_ind);

        int bottom_vert_ind = num_verts_in_circle * num_circles + 1;
        int actual_vert_ind_bottom1 = vert_ind;
        int actual_vert_ind_bottom2 = (vert_ind + 1) % num_verts_in_circle;
        inds.push_back(actual_vert_ind_bottom2);
        inds.push_back(actual_vert_ind_bottom1);
        inds.push_back(bottom_vert_ind);
    }
    assert(verts.size() == ret->vertex_count * NUM_FLOATS_PER_VERTEX);
    assert(inds.size() == ret->index_count);
    ret->vertices = (GLfloat *)malloc(ret->get_size_bytes());
    ret->indices = (GLushort *)malloc(ret->get_indices_size_bytes());
    memcpy(ret->vertices, verts.data(), ret->get_size_bytes());
    memcpy(ret->indices, inds.data(), ret->get_indices_size_bytes());
    return ret;
}

ShapeOnGPU ShapeRenderer::add_shape(Shape *shape) {
    ShapeOnGPU ret;
    ret.offset_vertices = tot_offset_vertices;
    ret.num_vertices = shape->vertex_count;
    ret.offset_indices = tot_offset_indices;
    ret.num_indices = shape->index_count;

    shapes.push_back(shape);
    shapes_on_gpu.push_back(ret);
    tot_offset_vertices += shape->vertex_count;
    tot_offset_indices += shape->index_count;

    return ret;
}
void ShapeRenderer::send_shapes() {
    if (sent_already) {
        std::cerr << "Cannot send shapes twice! Please add all shapes "
                     "before sending"
                  << std::endl;
        exit(1);
    }
    sent_already = true;
    if (!shapes.size())
        return;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    Shape *combined_shape = new Shape();
    combined_shape->num_floats_per_vertex = shapes[0]->num_floats_per_vertex;
    combined_shape->vertex_count = tot_offset_vertices;
    combined_shape->vertices =
        (GLfloat *)malloc(combined_shape->get_size_bytes());
    combined_shape->index_count = tot_offset_indices;
    combined_shape->indices =
        (GLushort *)malloc(combined_shape->get_indices_size_bytes());

    int tot_byte_offset_vertices = 0;
    int tot_num_indices = 0;
    int tot_num_vertices = 0;
    for (Shape *shape : shapes) {
        memcpy((char *)((uint64_t)combined_shape->vertices +
                        (uint64_t)tot_byte_offset_vertices),
               shape->vertices, shape->get_size_bytes());

        for (int i = 0; i < shape->index_count; i++) {
            combined_shape->indices[tot_num_indices + i] =
                tot_num_vertices + shape->indices[i];
        }

        tot_byte_offset_vertices += shape->get_size_bytes();
        tot_num_indices += shape->index_count;
        tot_num_vertices += shape->vertex_count;
    }
    // for (int i = 0; i < 6; i++) {
    //     std::cout << combined_shape->indices[i] << std::endl;
    // }

    // generate array buffer and send vertex data
    glGenBuffers(1, &myBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, myBufferID);
    glBufferData(GL_ARRAY_BUFFER, combined_shape->get_size_bytes(),
                 combined_shape->vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          combined_shape->get_stride_bytes(), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          combined_shape->get_stride_bytes(),
                          (void *)(combined_shape->get_color_offset()));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
                          combined_shape->get_stride_bytes(),
                          (void *)(combined_shape->get_normal_offset()));

    // generate element array buffer and send index data
    glGenBuffers(1, &myElementBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myElementBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 combined_shape->get_indices_size_bytes(),
                 combined_shape->indices, GL_STATIC_DRAW);
}
void ShapeRenderer::render_shape(ShapeOnGPU shape_gpu) {
    // glDrawArrays(GL_TRIANGLES, shape_gpu.offset_vertices,
    //              shape_gpu.num_vertices);
    glDrawElements(GL_TRIANGLES, shape_gpu.num_indices, GL_UNSIGNED_SHORT,
                   (void *)(shape_gpu.offset_indices * sizeof(GLushort)));
}

Camera::Camera() {
    position = glm::vec3(0, 0, 0);
    up = glm::vec3(0, 1, 0);
    front = glm::vec3(0, 0, -1);
    fov = glm::pi<float>() / 3;
    right = glm::cross(front, up);
}

Camera::Camera(std::string vertex_shader_src_file_name,
               std::string fragment_shader_src_file_name)
    : Camera() {
    std::ifstream vertexShaderFile(vertex_shader_src_file_name);
    if (!vertexShaderFile.is_open()) {
        std::cerr << "Error: Unable to open vertex shader file" << std::endl;
    }
    std::string vertexShaderSrc(
        (std::istreambuf_iterator<char>(vertexShaderFile)),
        std::istreambuf_iterator<char>());
    std::ifstream fragmentShaderFile(fragment_shader_src_file_name);
    if (!fragmentShaderFile.is_open()) {
        std::cerr << "Error: Unable to open fragment shader file" << std::endl;
    }
    std::string fragmentShaderSrc(
        (std::istreambuf_iterator<char>(fragmentShaderFile)),
        std::istreambuf_iterator<char>());

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char *vertex_src = vertexShaderSrc.c_str();
    glShaderSource(vertexShader, 1, &vertex_src, 0);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fragment_src = fragmentShaderSrc.c_str();
    glShaderSource(fragmentShader, 1, &fragment_src, 0);
    glCompileShader(fragmentShader);

    GLint compileStatus;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE) {
        GLint infoLength;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLength);
        GLchar *buffer = new GLchar[infoLength];

        GLsizei bufferSize;
        glGetShaderInfoLog(vertexShader, infoLength, &bufferSize, buffer);
        std::cerr << buffer << std::endl;
        delete[] buffer;
    }
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE) {
        GLint infoLength;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLength);
        GLchar *buffer = new GLchar[infoLength];

        GLsizei bufferSize;
        glGetShaderInfoLog(fragmentShader, infoLength, &bufferSize, buffer);
        std::cerr << buffer << std::endl;
        delete[] buffer;
    }

    GLuint programObject = glCreateProgram();

    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);
    glLinkProgram(programObject);
    glValidateProgram(programObject);

    glEnable(GL_DEPTH_TEST);
    // glDisable(GL_CULL_FACE);
    glEnable(GL_CULL_FACE);
    glUseProgram(programObject);

    full_transform_matrix_location =
        glGetUniformLocation(programObject, "fullTransformMatrix");
    light_direction_uniform_location =
        glGetUniformLocation(programObject, "lightDirection");
}
glm::mat4 Camera::get_world_to_perspective_transform_matrix() {
    glm::vec3 looking_vector =
        glm::vec3(glm::rotate(glm::mat4(1), looking_angle, right) *
                  glm::vec4(front, 1.0));
    return glm::perspective(fov, get_global_aspect_ratio(), near_plane,
                            far_plane) *
           glm::lookAt(position, position + looking_vector, up);
}
void Camera::rotate_left_right(float angle) {
    front =
        glm::vec3(glm::rotate(glm::mat4(1), angle, up) * glm::vec4(front, 1.0));
    right = glm::cross(front, up);
}
void Camera::rotate_up_down(float angle) {
    looking_angle += angle;
    looking_angle = glm::clamp(looking_angle, -glm::half_pi<float>() + 0.00001f,
                               glm::half_pi<float>() - 0.00001f);
}
void Camera::move_front(float distance) { position += front * distance; }
void Camera::strafe(float distance) { position += right * distance; }
void Camera::move_up(float distance) { position += up * distance; }
void Camera::track_input(Keys keys, MouseState mouse_state, float dt) {
    float distance = movement_speed * dt;
    if (keys[K_ctrl]) {
        distance *= 3;
    }
    if (keys[K_w]) {
        move_front(distance);
    }
    if (keys[K_s]) {
        move_front(-distance);
    }
    if (keys[K_a]) {
        strafe(-distance);
    }
    if (keys[K_d]) {
        strafe(distance);
    }
    if (keys[K_shift]) {
        move_up(-distance);
    }
    if (keys[K_space]) {
        move_up(distance);
    }
    if (minecraft_rotation) {
        rotate_left_right(-(mouse_state.x - prev_mouse_state.x) * 2.0 /
                          global_width);
        rotate_up_down(-(mouse_state.y - prev_mouse_state.y) * 2.0 /
                       global_width);
        SDL_WarpMouseInWindow(global_window, global_width / 2,
                              global_height / 2);
        prev_mouse_state.x = global_width / 2;
        prev_mouse_state.y = global_height / 2;
    } else {
        if (mouse_state.pressed_left()) {
            rotate_left_right((mouse_state.x - prev_mouse_state.x) * 2.0 /
                              global_width);
            rotate_up_down((mouse_state.y - prev_mouse_state.y) * 2.0 /
                           global_width);
        }
        prev_mouse_state = mouse_state;
    }
}
void Camera::toggle_minecraft_rotation() {
    if (!minecraft_rotation) {
        minecraft_rotation = true;
        SDL_ShowCursor(SDL_DISABLE);
    } else {
        minecraft_rotation = false;
        SDL_ShowCursor(SDL_ENABLE);
    }
}
void Camera::draw_shape(glm::mat4 model_transform_matrix,
                        ShapeOnGPU shape_gpu) {
    glm::mat4 full_transform_matrix =
        get_world_to_perspective_transform_matrix() * model_transform_matrix;
    glm::vec3 light_direction(1.0f, 3.0f, 2.0f);
    light_direction = glm::normalize(light_direction);
    glUniformMatrix4fv(full_transform_matrix_location, 1, GL_FALSE,
                       &full_transform_matrix[0][0]);
    glUniform3fv(light_direction_uniform_location, 1, &light_direction[0]);
    ShapeRenderer::render_shape(shape_gpu);
}

Object::Object(ShapeOnGPU shape_on_gpu) {
    this->shape_on_gpu = shape_on_gpu;
    position = glm::vec3(0.0f, 0.0f, -3.0f);
}
glm::mat4 Object::get_model_to_world_transform_matrix() {
    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position);
    return translation_matrix;
}
void Object::draw(Camera &camera) {
    camera.draw_shape(get_model_to_world_transform_matrix(),
                      this->shape_on_gpu);
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
                default:
                    break;
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
    Pos2D arrow_center = {rect.x + rect.w - rect.h / 3.0f,
                          rect.y + rect.h / 2.0f};
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
