#include "cygame.h"
#include "glad/glad.h"
#include <iostream>

using namespace std;
using namespace glm;

// this is just a demo to show how to use cygame.

// this is the callback function that the button calls.
int main() {
    // initialises sdl.
    cygame_init();

    make_opengl_screen(1000, 700, 1, "Cygame opengl demo!");
    bool running = true;

    Shape *cube = ShapeGenerator::get_cube();
    Shape *sphere = ShapeGenerator::get_sphere(200, 100);
    // Shape *sphere = ShapeGenerator::get_cube();
    Shape *monkey = ShapeGenerator::get_from_file("models/monkey.obj");
    // for (int i = 0; i < monkey->vertex_count; i++) {
    //     cout << "vert: " << monkey->vertices[NUM_FLOATS_PER_VERTEX * i +
    //     0]
    //          << ' ' << monkey->vertices[NUM_FLOATS_PER_VERTEX * i + 1] <<
    //          ' '
    //          << monkey->vertices[NUM_FLOATS_PER_VERTEX * i + 2] << endl;
    //     cout << "colr: " << monkey->vertices[NUM_FLOATS_PER_VERTEX * i +
    //     3]
    //          << ' ' << monkey->vertices[NUM_FLOATS_PER_VERTEX * i + 4] <<
    //          ' '
    //          << monkey->vertices[NUM_FLOATS_PER_VERTEX * i + 5] << endl;
    //     cout << "norm: " << monkey->vertices[NUM_FLOATS_PER_VERTEX * i +
    //     6]
    //          << ' ' << monkey->vertices[NUM_FLOATS_PER_VERTEX * i + 7] <<
    //          ' '
    //          << monkey->vertices[NUM_FLOATS_PER_VERTEX * i + 8] << endl;
    //     cout << endl;
    // }
    // for (int i = 0; i < monkey->index_count; i++) {
    //     cout << monkey->indices[i] << ' ';
    //     if (i % 3 == 2)
    //         cout << endl;
    // }
    // monkey->vertex_count = 4;
    // monkey->index_count = 6;

    ShapeRenderer renderer;
    auto cube_gpu = renderer.add_shape(cube);
    auto sphere_gpu = renderer.add_shape(sphere);
    auto monkey_gpu = renderer.add_shape(monkey);

    // creating the shader program

    Camera camera("vertex_shader_demo.glslv", "fragment_shader_demo.glslf");

    renderer.send_shapes();

    Object cube_object(cube_gpu);
    Object cube_object_2(cube_gpu);
    Object cube_object_3(cube_gpu);
    Object sphere_object(sphere_gpu);
    Object monkey_object(monkey_gpu);
    cube_object_2.position.z = -7;
    cube_object_3.position.x = -7;
    sphere_object.position.z = -7;
    monkey_object.position.x = 7;

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
            case K_m:
                camera.toggle_minecraft_rotation();
            default:
                break;
            }
        }

        // gets a list of keys that are currently held down (Keys is Uint8 *).
        // You can use this instead of keydown events to detect keys being
        // pressed, because it's faster.

        Keys keys = get_keys_pressed();

        if (keys[K_t]) {
            cube_object_2.position.x -= 0.05;
        }

        // this is a similar thing for the mouse input.

        MouseState mouse_state = get_mouse_state();

        camera.track_input(keys, mouse_state, 1.0 / 60.0);

        clear_opengl_screen({0, 0, 0, 0});

        sphere_object.draw(camera);
        cube_object.draw(camera);
        monkey_object.draw(camera);
        // cube_object_2.draw(camera);
        // cube_object_3.draw(camera);

        // just swaps the buffers
        draw_opengl_screen();

        delay(1000 / 60);
    }
    return 0;
}
