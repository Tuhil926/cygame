#include "cygame.h"
#include "glad/glad.h"
using namespace std;

int main() {
    // initialises sdl.
    cygame_init();

    // The sdl renderer. everything will be drawn onto this.
    make_screen(700, 700, 1, "CYGame demo!");

    ShapeRenderer renderer;

    Camera camera("demo_shader.glslv", "demo_shader.glslf");

    GLuint time_uniform =
        glGetUniformLocation(camera.programObject, "timeSeconds");

    float time = 0;
    renderer.send_shapes();

    bool running = true;
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
                // printf("You pressed w!\n");
            default:
                break;
            }
        }

        // you need to clear the screen every frame, or things will create
        // trails of their previous renders. you can comment this out to see
        // what I mean
        clear_screen({0, 0, 0, 0});

        glUniform1f(time_uniform, time);
        camera.draw_rect({0, 0, 700, 700}, {255, 255, 255, 255});

        // finally, you need to call draw_screen to show the stuff onto the
        // screen.
        draw_screen();

        delay(1000 / 60.0);
        time += 1 / 60.0;
    }
    return 0;
}
