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

    ShapeRenderer renderer;
    auto cube_gpu = renderer.add_shape(cube);

    // creating the shader program

    Camera camera("vertex_shader_demo.glslv", "fragment_shader_demo.glslf");

    renderer.send_shapes();

    Object cube_object(cube_gpu);
    Object cube_object_2(cube_gpu);
    Object cube_object_3(cube_gpu);
    cube_object_2.position.z = -7;
    cube_object_3.position.x = -7;

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

        cube_object.draw(camera);
        cube_object_2.draw(camera);
        cube_object_3.draw(camera);

        // just swaps the buffers
        draw_opengl_screen();

        delay(1000 / 60);
    }
    return 0;
}
