#include "cygame.h"
#include "glad/glad.h"
#include "glm/ext/vector_float4.hpp"
#include <fstream>
#include <iostream>

using namespace std;
using namespace glm;

// this is just a demo to show how to use cygame.

// this is the callback function that the button calls.
int main() {
    // initialises sdl.
    cygame_init();

    CYGLScreen screen = make_opengl_screen(1000, 700, 1, "Cygame opengl demo!");
    bool running = true;

    Shape *triangle = ShapeGenerator::get_triangle();
    Shape *triangle2 = ShapeGenerator::get_triangle2();

    ShapeRenderer renderer;
    auto tri_gpu = renderer.add_shape(triangle);
    auto tri_gpu2 = renderer.add_shape(triangle2);

    // creating the shader program

    ifstream vertexShaderFile("vertex_shader_demo.glslv");
    if (!vertexShaderFile.is_open()) {
        cerr << "Error: Unable to open vertex shader file" << endl;
    }
    string vertexShaderSrc((istreambuf_iterator<char>(vertexShaderFile)),
                           istreambuf_iterator<char>());
    ifstream fragmentShaderFile("fragment_shader_demo.glslf");
    if (!fragmentShaderFile.is_open()) {
        cerr << "Error: Unable to open fragment shader file" << endl;
    }
    string fragmentShaderSrc((istreambuf_iterator<char>(fragmentShaderFile)),
                             istreambuf_iterator<char>());

    GLuint programObject = glCreateProgram();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char *vertex_src = vertexShaderSrc.c_str();
    glShaderSource(vertexShader, 1, &vertex_src, 0);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fragment_src = fragmentShaderSrc.c_str();
    glShaderSource(fragmentShader, 1, &fragment_src, 0);
    glCompileShader(fragmentShader);

    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);
    glLinkProgram(programObject);
    glValidateProgram(programObject);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glUseProgram(programObject);

    // glBindVertexArray(renderer.vertexArrayObject);
    // glBindBuffer(GL_ARRAY_BUFFER, renderer.myBufferID);
    renderer.send_shapes();
    while (running) {
        // you need to use this handle_event macro if you want to be able to use
        // the input box. don't ask me why. It's just convenient and easier than
        // calling the sdl functions. Oh also, it's a switch statement, so you
        // need to hadle the cases.
        handle_events {
        // you can also use any of the sdl2 specific cases if you need to.
        case QUIT:
            running = false;
            break;

        case KEYDOWN:
            // this is also a useful macro, and it must be used along with
            // handle_events, because they use the _event variable. This is
            // a switch statement on the keycode
            handle_keycode {
            case K_w:
                printf("You pressed w!\n");
            default:
                break;
            }
        }

        // gets a list of keys that are currently held down (Keys is Uint8 *).
        // You can use this instead of keydown events to detect keys being
        // pressed, because it's faster.

        Keys keys = get_keys_pressed();

        // this is a similar thing for the mouse input.

        MouseState mouse_state = get_mouse_state();

        // opengl stuff
        clear_opengl_screen({0, 0, 0, 0});

        renderer.render_shape(tri_gpu);
        renderer.render_shape(tri_gpu2);

        // just swaps the buffers
        draw_opengl_screen();

        delay(1000 / 60);
    }
    return 0;
}
