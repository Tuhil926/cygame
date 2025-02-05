#include "cygame.h"
#include "glad/glad.h"
using namespace std;

char fragment_shader[] =
    "#version 410 core\n"
    "in vec3 theColor;\n"
    "in vec3 theNormal;\n"
    "in vec3 pos;\n"
    "in vec2 textureCoord;\n"
    "out vec4 color;\n"
    "uniform vec3 lightDirection;\n"
    "uniform vec3 cameraLocation;\n"
    "uniform sampler2D text;\n"
    "uniform int isTexture;\n"
    "uniform float specMultiplier;\n"
    "uniform float timeSeconds;\n"
    "float norm(vec3 c){\n"
    "    return sqrt(c.x*c.x + c.y*c.y);\n"
    "}\n"
    "float a1(vec3 p, float phase){\n"
    "    return sin(1./(1. - abs(norm(p) + tan(phase/10.))));\n"
    "}\n"
    "vec3 b(vec3 p, float phase, float angle){\n"
    "    p = p*(1 + sin(angle*100)/50.);\n"
    "    return vec3(clamp(a1(p, phase), 0., 1.) + clamp(tan((angle - phase)), "
    "0., 1.), a1(p, phase + 0.3), a1(p, phase + 0.6));\n"
    "}\n"
    "vec4 c(vec3 p, float phase){\n"
    "    float angle = atan(p.y/p.x);\n"
    "    vec3 p1 = p*(1 + sin(angle*100)/50.);\n"
    "    vec3 b_ret = vec3(a1(p1, phase), a1(p1, phase + 0.1), a1(p, phase + "
    "0.2));\n"
    "    float val = clamp(.1/tan(phase - angle), 0., 1.);\n"
    "    return clamp(vec4(b_ret, 1.)/(norm(p*4.)) + vec4(val, val*val/2., "
    "val*val*val/2., 1.), 0., 1.);\n"
    "}\n"
    "vec4 c2(vec3 p, float phase){\n"
    "    float angle = -atan(p.y/p.x);\n"
    "    vec3 p1 = p*(1 + sin(angle*100)/50.);\n"
    "    vec3 b_ret = vec3(a1(p1, phase), a1(p1, phase + 0.1), a1(p, phase + "
    "0.2));\n"
    "    float val = clamp(.1/tan(angle - phase), 0., 1.);\n"
    "    return clamp(vec4(b_ret, 1.)/(norm(p*4.)) + vec4(val, val*val/2., "
    "val*val*val/2., 1.), 0., 1.);\n"
    "}\n"
    "vec4 c3(vec3 p, float phase){\n"
    "    float angle = atan(p.y/p.x);\n"
    "    vec3 p1 = p*(1 + sin(angle*100)/50.);\n"
    "    vec3 b_ret = vec3(a1(p1, phase), a1(p1, phase + 0.1), a1(p, phase + "
    "0.2));\n"
    "    float val = clamp(.1/tan(angle - phase), 0., 1.);\n"
    "    return clamp(vec4(b_ret, 1.)/(norm(p*4.)) + vec4(val, val*val/2., "
    "val*val*val/2., 1.), 0., 1.);\n"
    "}\n"
    "void main() {\n"
    "    vec3 pos1 = pos*2.;\n"
    "    color = c(pos1 , timeSeconds + 50.);\n"
    "    color += c2(pos1 + vec3(1., 1., 0.), -timeSeconds);\n"
    "    color += c2(pos1 - vec3(1., 1., 0.), -timeSeconds);\n"
    "    color += c3(pos1 + vec3(-1., 1., 0.), -timeSeconds);\n"
    "    color += c3(pos1 - vec3(-1., 1., 0.), -timeSeconds);\n"
    "}\n";

int main() {
    // initialises sdl.
    cygame_init();

    // The sdl renderer. everything will be drawn onto this.
    make_screen(700, 700, 1, "CYGame demo!");

    ShapeRenderer renderer;

    set_default_fragment_shader(fragment_shader);
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
        camera.draw_rect(
            {0, 0, (int)get_global_width(), (int)get_global_height()},
            {255, 255, 255, 255});

        // finally, you need to call draw_screen to show the stuff onto the
        // screen.
        draw_screen();

        delay(1000 / 144.0);
        time += 1 / 144.0;
    }
    return 0;
}
