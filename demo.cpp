#include "cygame.h"

// this is just a demo to show how to use cygame.

// this is the callback function that the button calls.
void hello(void *rd) {
    printf("Hello!\n");
    unsigned char *r = (unsigned char *)rd;
    *r = *r + 100;
}

int main() {
    // initialises sdl.
    cygame_init();

    // The sdl renderer. everything will be drawn onto this.
    CYScreen screen = make_screen(1000, 700, 1, "CYGame demo!");

    bool running = true;

    // just some stuff for demo purposes
    int x = 0, y = 0;

    unsigned char rd = 100;

    Button button =
        Button({700, 300, 150, 60}, "hello", 24, {190, 180, 70, 255},
               {220, 210, 100, 255}, {255, 255, 150, 255}, hello, &rd);
    InputBox input({100, 300, 350, 60}, "sup", 24, {200, 200, 200, 255},
                   {220, 220, 220, 255}, {120, 120, 120, 255});
    Slider slider = Slider({400, 200}, {800, 200}, 100, {200, 200, 200, 255},
                           {255, 255, 255, 255}, 40);
    Slider slider2 = Slider({500, 250}, {800, 650}, 100, {100, 200, 100, 255},
                            {255, 255, 255, 255}, 40);
    StaticText text = StaticText({100, 600}, "This is some text", 30,
                                 {100, 100, 255, 255}, screen, false);
    StaticText text2 =
        StaticText({500, 50}, "Sum", 32, {100, 100, 255, 255}, screen);

    Selector selector = Selector({0, 0, 200, 100}, 24, {"opt1", "opt2", "opt3"},
                                 {100, 100, 100, 255}, {120, 120, 120, 255},
                                 {130, 130, 130, 255});

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

        // just some demo stuff, it moves the square around
        if (keys[K_w])
            y -= 5;
        if (keys[K_a])
            x -= 5;
        if (keys[K_s])
            y += 5;
        if (keys[K_d])
            x += 5;

        // this is important. Every object that you create has an update
        // function that you need to call every frame. if you don't call it, it
        // won't update (duh).
        button.update(mouse_state);
        input.update(mouse_state, _events, keys);
        slider.update(mouse_state);
        slider2.update(mouse_state);
        selector.update(mouse_state);
        text2.set_text("Sum: " +
                       std::to_string((int)(slider.value + slider2.value)));

        // you need to clear the screen every frame, or things will create
        // trails of their previous renders. you can comment this out to see
        // what I mean
        clear_screen(screen);

        fill_rect({x, y, 100, 100}, screen,
                  {rd, rd, (unsigned char)(255 - rd), 255});

        draw_gradient_polygon(
            screen, {{{slider.value * 8, slider2.value * 7},
                      {255, 255, 0, 255},
                      {0, 0}},
                     {{350, (float)(350 - slider2.value * 4) + 175},
                      {100, 255, 0, 255},
                      {0, 0}},
                     {{slider.value * 3 + slider2.value * 2, 600},
                      {200, 100, 0, 255},
                      {0, 0}},
                     {{slider.value * 1, 600}, {200, 0, 0, 255}, {0, 0}}});

        // every object also has a draw method that you need to call. if you
        // don't want to draw it, don't call the method.
        button.draw(screen);
        input.draw(screen);
        slider.draw(screen);
        slider2.draw(screen);
        text.draw();
        text2.draw();
        selector.draw(screen);

        // finally, you need to call draw_screen to show the stuff onto the
        // screen.
        draw_screen(screen);

        delay(1000 / 60);
    }
    return 0;
}
