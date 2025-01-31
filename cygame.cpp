#include "cygame.h"
#include "SDL_events.h"
#include "SDL_mouse.h"
#include "SDL_video.h"
#include "glad/glad.h"
#include <iostream>
#include <ostream>
#include <vector>

// initialises SDL2
int cygame_init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    // TTF_Init();
    std::cout << "Welcome to the cygame community (estimated size: 1 person)\n";
    return 0;
}
SDL_Window *global_window;
CYGLScreen global_gl_context = NULL;
float global_width, global_height;

SDL_Window *get_global_window() { return global_window; }
CYGLScreen get_global_gl_context() { return global_gl_context; }
float get_global_width() { return global_width; }
float get_global_height() { return global_height; }

/// @brief makes an opengl screen and returns the opengl context. You dont need
/// to store the opengl context anywhere, it's global to opengl
/// @param width
/// @param height
/// @param gui_scale the current gui scale of your laptop. 1 by default, but if
/// it's set to 1.5 for example, then the final screen will be scaled down
/// by 1.5
/// @param title
/// @return CYGLScreen
void make_screen(int width, int height, float gui_scale, const char *title) {
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
    global_gl_context = SDL_GL_CreateContext(win);

    int err = gladLoadGLLoader(SDL_GL_GetProcAddress);
    if (!err) {
        printf("error initializing glad: %d\n", err);
        exit(1);
    }

    glViewport(0, 0, width, height);
    const unsigned char *glsl_version =
        glGetString(GL_SHADING_LANGUAGE_VERSION);
    if (glsl_version)
        std::cout << glsl_version << std::endl;
    else
        std::cout << "glsl version could not be detected, it looks like glad "
                     "is not working properly "
                     "(glGetString(GL_SHADING_LANGUAGE_VERSION) returned 0)\n";

    // SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN_DESKTOP);
}

// gets a list of keys that are currently held down (Keys is Uint8 *).
// You can use this instead of keydown events to detect keys being
// pressed, because it's faster.
Keys get_keys_pressed() { return SDL_GetKeyboardState(0); }

// just calls SDL_Delay, which sleeps for m milliseconds. Idk why I wrote this
// function, I guess it just makes it convenient to type out and consistent with
// the rest of stuff by removing the SDL prefix. You don't have to use this tho
void delay(int m) { SDL_Delay(m); }

void clear_screen(Color color) {
    glClearColor(color.r / 255.0, color.g / 255.0, color.b / 255.0,
                 color.a / 255.0);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

float get_global_aspect_ratio() {
    return get_global_width() / (float)get_global_height();
}

void draw_screen() {
    // set_global_dimensions_to_window_width(screen);
    // SDL_GL_MakeCurrent(get_global_window(), get_global_gl_context());
    int w, h;
    SDL_GetWindowSize(global_window, &w, &h);
    global_width = w;
    global_height = h;
    glViewport(0, 0, global_width, global_height);
    SDL_GL_SwapWindow(get_global_window());
}

// returns the current state of the mouse, which are it's x and y coordinates
// with respect to the window, as well as information about what mouse buttons
// are being pressed. I fixed a bug in SDL2 where the SDL_GetMouseState doesn't
// take into account that the screen has been scaled. I just re calculate the x
// and y values and return them.
MouseState get_mouse_state() {
    int x, y;
    Uint32 buttons = SDL_GetMouseState(&x, &y);

    int w, h;
    SDL_GetWindowSize(get_global_window(), &w, &h);
    // std::cout << w << " " << h << std::endl;
    // std::cout << global_width << " " << global_height << std::endl;
    float fx = x, fy = y, fw = w, fh = h, global_width = get_global_width(),
          global_height = get_global_height();
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
