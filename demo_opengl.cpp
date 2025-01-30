#include "cygame.h"

using namespace std;
using namespace glm;

// this is just a demo to show how to use cygame.

// this is the callback function that the button calls.
int main() {
    // initialises sdl.
    cygame_init();

    auto screen = make_opengl_screen(1000, 700, 1, "Cygame opengl demo!");
    bool running = true;

    Shape *cube = ShapeGenerator::get_cube();
    Shape *sphere = ShapeGenerator::get_sphere(200, 100);
    // Shape *sphere = ShapeGenerator::get_cube();
    Shape *plane = ShapeGenerator::get_from_file("models/plane.obj");

    ShapeRenderer renderer;
    auto cube_gpu = renderer.add_shape(cube);
    auto sphere_gpu = renderer.add_shape(sphere);
    auto plane_gpu = renderer.add_shape(plane);

    // creating the shader program

    Camera camera("vertex_shader_demo.glslv", "fragment_shader_demo.glslf");

    renderer.send_shapes();

    Object cube_object(cube_gpu);
    Object cube_object_2(cube_gpu);
    Object sphere_object(sphere_gpu);
    Object plane_object(plane_gpu);
    cube_object_2.position.z = -7;
    sphere_object.position.z = -7;
    plane_object.position.x = 7;
    plane_object.scale.x = 7;
    plane_object.scale.y = 7;
    plane_object.scale.z = 7;

    // plane_object.rotate_x(1.0);
    plane_object.rotate_y(0.5);
    plane_object.rotate_x(1.0);

    auto font = Font(DEFAULT_FONT, 16);

    Text text({500, 30}, "press m to toggle between mouse mode and camera mode",
              &font, {0, 255, 255, 255});
    Button button1({50, 50, 100, 50}, "hello", &font, {150, 150, 60, 255},
                   {200, 200, 100, 255}, {255, 255, 150, 255}, NULL);

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
        button1.update(mouse_state);

        // drawing things

        clear_opengl_screen({0, 0, 0, 0});

        sphere_object.draw(camera);
        cube_object.draw(camera);
        plane_object.draw(camera);
        // camera.render_text(
        //     "press w to toggle between mouse mode and camera mode", 170, 30,
        //     {0.0, 1.0, 1.0}, &font);
        text.draw(camera);

        camera.draw_rect({0, 600, 100, 100}, {255, 0, 255, 255});

        button1.draw(camera);

        camera.draw_line({100, 100}, {500, 500}, {255, 255, 255, 255}, 0.5);

        camera.draw_circle({600, 600}, 50, {255, 0, 0, 255});

        // cube_object_2.draw(camera);

        // sets the viewport size to window size and swaps the frame buffer to
        // display what was drawn
        draw_opengl_screen(screen);

        delay(1000 / 60);
    }
    font.cleanup();
    return 0;
}
