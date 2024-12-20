#include "cygame.h"
#include "glad/glad.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/gtc/constants.hpp"
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
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
    Shape *cube = ShapeGenerator::get_cube();

    ShapeRenderer renderer;
    auto tri_gpu = renderer.add_shape(triangle);
    auto tri_gpu2 = renderer.add_shape(triangle2);
    auto cube_gpu = renderer.add_shape(cube);

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
        cout << buffer << endl;
        delete[] buffer;
    }
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE) {
        GLint infoLength;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLength);
        GLchar *buffer = new GLchar[infoLength];

        GLsizei bufferSize;
        glGetShaderInfoLog(fragmentShader, infoLength, &bufferSize, buffer);
        cout << buffer << endl;
        delete[] buffer;
    }

    GLuint programObject = glCreateProgram();

    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);
    glLinkProgram(programObject);
    glValidateProgram(programObject);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glUseProgram(programObject);

    vec3 camera_position(0.0f, 0.0f, 0.0f);

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
            // case K_w:
            //     printf("You pressed w!\n");
            default:
                break;
            }
        }

        // gets a list of keys that are currently held down (Keys is Uint8 *).
        // You can use this instead of keydown events to detect keys being
        // pressed, because it's faster.

        Keys keys = get_keys_pressed();

        if (keys[K_w]) {
            camera_position.z -= 0.01;
        }
        if (keys[K_s]) {
            camera_position.z += 0.01;
        }
        if (keys[K_a]) {
            camera_position.x -= 0.01;
        }
        if (keys[K_d]) {
            camera_position.x += 0.01;
        }
        if (keys[K_shift]) {
            camera_position.y -= 0.01;
        }
        if (keys[K_space]) {
            camera_position.y += 0.01;
        }

        // this is a similar thing for the mouse input.

        MouseState mouse_state = get_mouse_state();

        mat4 model_transform_matrix =
            glm::translate(mat4(1), vec3(0, 0, -4)) *
            glm::rotate(mat4(1), quarter_pi<float>(), vec3(1.0, 1.0, 0.0));
        mat4 world_to_view_matrix = lookAt(
            camera_position, camera_position + vec3(0, 0, -1), vec3(0, 1, 0));
        mat4 projection_matrix =
            glm::perspective(pi<float>() / 3, 1000.0f / 700.0f, 0.1f, 10.0f);

        mat4 full_transform_matrix =
            projection_matrix * world_to_view_matrix * model_transform_matrix;

        // for (int i = 0; i < 4; i++) {
        //     for (int j = 0; j < 4; j++) {
        //         cout << full_transform_matrix[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;
        // cout << endl;

        GLint full_transform_matrix_location =
            glGetUniformLocation(programObject, "fullTransformMatrix");
        // cout << full_transform_matrix_location << endl;
        glUniformMatrix4fv(full_transform_matrix_location, 1, GL_FALSE,
                           &full_transform_matrix[0][0]);

        // opengl stuff
        clear_opengl_screen({0, 0, 0, 0});

        // renderer.render_shape(tri_gpu);
        // renderer.render_shape(tri_gpu2);
        renderer.render_shape(cube_gpu);

        // just swaps the buffers
        draw_opengl_screen();

        delay(1000 / 60);
    }
    return 0;
}
