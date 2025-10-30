#include "cygame.h"
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
using namespace std;

const int SCREEN_WIDTH = 1900;
const int SCREEN_HEIGHT = 1000;

class VisualNode {
  public:
    float x;
    float y;
    Pos2D vel{0, 0};
    VisualNode() : x(0), y(0) {}
    VisualNode(float x, float y) : x(x), y(y) {}
    VisualNode(int x, int y) : x(x), y(y) {}
    Text text;
    float radius = 25;
    bool clicked = false;

    void set_text(string text, Font *font) {
        this->text = Text({x, y}, text, font, {0, 0, 0});
    }

    void draw(Camera &camera, Pos2D camera_pos) {
        camera.draw_circle({x - camera_pos.x, y - camera_pos.y}, radius,
                           {255, 255, 255});
        this->text.pos = {x - camera_pos.x, y - camera_pos.y};
        this->text.draw(camera);
    }
    void update(Pos2D camera_pos, MouseState mouse_state,
                bool already_clicked) {
        Pos2D mouse_pos = mouse_state.position();
        x += vel.x;
        y += vel.y;
        vel = {0, 0};
        if (mouse_state.pressed_left()) {
            if (((Pos2D{x, y} - camera_pos) - mouse_pos).norm() <= radius and
                !already_clicked)
                clicked = true;

            if (clicked) {
                x = mouse_pos.x + camera_pos.x;
                y = mouse_pos.y + camera_pos.y;
            }
        } else {
            clicked = false;
        }
        if (x < -10000)
            x = 0;
        if (y < -10000)
            y = 0;
        if (x > 10000)
            x = 10000;
        if (y > 10000)
            y = 10000;
        // if (x < 0)
        //     x = 0;
        // if (y < 0)
        //     y = 0;
        // if (x > SCREEN_WIDTH)
        //     x = SCREEN_WIDTH;
        // if (y > SCREEN_HEIGHT)
        //     y = SCREEN_HEIGHT;
    }
};

int main() {
    // initialises sdl.
    int N = 5, M = 5;
    cout << "Enter Number of nodes: ";
    cin >> N;
    cout << "Enter number of edges: ";
    cin >> M;

    vector<VisualNode> nodes(N);
    vector<vector<int>> adj_list(N);

    srand(time(NULL));
    for (int i = 0; i < M; i++) {
        int a, b;
        // cin >> a >> b;
        // a--;
        // b--;
        bool duplicate = false;

        for (int l = 0; l < 10; l++) {
            duplicate = false;
            a = abs(rand()) % N;
            b = abs(rand()) % N;
            if (a == b)
                duplicate = true;
            for (int j = 0; j < adj_list[a].size(); j++) {
                if (adj_list[a][j] == b)
                    duplicate = true;
            }
            for (int j = 0; j < adj_list[b].size(); j++) {
                if (adj_list[b][j] == a)
                    duplicate = true;
            }
            if (!duplicate)
                break;
        }

        if (!duplicate) {
            adj_list[a].push_back(b);
            adj_list[b].push_back(a);
        }
    }

    // nodes[0] = {250, 250};
    // nodes[1] = {300, 200};
    // nodes[2] = {300, 300};
    // nodes[3] = {200, 300};
    // nodes[4] = {700, 250};
    //
    // adj_list[0].push_back(1);
    // adj_list[1].push_back(0);
    //
    // adj_list[1].push_back(2);
    // adj_list[2].push_back(1);
    //
    // adj_list[1].push_back(4);
    // adj_list[4].push_back(1);
    //
    // adj_list[2].push_back(3);
    // adj_list[3].push_back(2);
    //
    // adj_list[3].push_back(4);
    // adj_list[4].push_back(3);

    cygame_init();

    // The sdl renderer. everything will be drawn onto this.
    make_screen(SCREEN_WIDTH, SCREEN_HEIGHT, 1, "CYGame demo!");
    Font font("fonts/PixelOperator8.ttf", 24);

    // just some stuff for demo purposes
    int x = 0, y = 0;

    unsigned char rd = 100;

    for (int i = 0; i < N; i++) {
        nodes[i].set_text(to_string(i), &font);
        nodes[i].x = abs(rand()) % SCREEN_WIDTH;
        nodes[i].y = abs(rand()) % SCREEN_HEIGHT;
        cout << nodes[i].x << " " << nodes[i].y << endl;
    }

    // This is a font. You can create just one or two fonts and use it for
    // all consequent objects. I don't recommend creating one font per
    // object, as this can be expensive. I'll later add support for fonts to
    // be scaled so that you can just create one large font and use it to
    // render everything.
    Camera camera("vertex_shader_demo.glslv", "fragment_shader_demo.glslf");
    Pos2D pos_camera{0, 0};
    Pos2D pos_camera_old{0, 0};
    Pos2D mouse_clicked_pos;
    bool mouse_clicked = false;

    ShapeRenderer renderer;
    renderer.send_shapes();

    bool node_clicked = false;
    bool running = true;
    while (running) {
        // cout << "Node positions" << endl;
        // for (int i = 0; i < N; i++) {
        //     cout << nodes[i].x << " " << nodes[i].y << endl;
        // }
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
            break;
        case MOUSE_BUTTON_DOWN:
            mouse_clicked = true;
            break;
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
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < adj_list[i].size(); j++) {
                VisualNode node1 = nodes[i];
                VisualNode node2 = nodes[adj_list[i][j]];

                Pos2D diff = Pos2D{node2.x, node2.y} - Pos2D{node1.x, node1.y};

                float dist = diff.norm();

                // attractive forces
                float force = dist * 0.0001f;

                if (dist == 0) {
                    force = 1;
                    diff.x = 5;
                }
                // repulsion from adjacent
                force -= (0.1f / (0.1 + dist * dist * dist / (2500 * 50)));

                if (force && dist > 0) {
                    diff = diff * force;
                    nodes[i].vel = nodes[i].vel + diff;
                    // nodes[i].x += diff.x;
                    // nodes[i].y += diff.y;
                }

                // repulsive forces from double adjacent
                for (int k = 0; k < adj_list[i].size(); k++) {
                    if (k == j)
                        continue;
                    VisualNode node3 = nodes[adj_list[i][k]];
                    Pos2D diff2 =
                        Pos2D{node3.x, node3.y} - Pos2D{node2.x, node2.y};
                    float dist2 = diff2.norm();
                    if (dist2 > 0) {
                        diff2 = diff2 * (-200.0f / (dist2 * dist2));
                        nodes[adj_list[i][j]].vel =
                            nodes[adj_list[i][j]].vel + diff2;
                        // nodes[adj_list[i][j]].x += diff2.x;
                        // nodes[adj_list[i][j]].y += diff2.y;
                    }
                }
            }
        }

        for (int i = 0; i < N; i++) {
            nodes[i].update(pos_camera, mouse_state, node_clicked);
            if (nodes[i].clicked)
                node_clicked = true;
        }

        if (mouse_clicked and !node_clicked) {
            pos_camera_old = pos_camera;
            mouse_clicked_pos = mouse_state.position();
            mouse_clicked = false;
        }
        if (mouse_state.pressed_left() and !node_clicked) {
            pos_camera =
                pos_camera_old + (mouse_clicked_pos - mouse_state.position());
        }
        if (!mouse_state.pressed_left()) {
            node_clicked = false;
        }

        clear_screen({0, 0, 0, 125});

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < adj_list[i].size(); j++) {
                VisualNode node1 = nodes[i];
                VisualNode node2 = nodes[adj_list[i][j]];
                Pos2D pos1{node1.x, node1.y};
                Pos2D pos2{node2.x, node2.y};
                camera.draw_line(pos1 - pos_camera, pos2 - pos_camera,
                                 {100, 100, 100}, 1);
            }
        }
        for (auto node : nodes) {
            node.draw(camera, pos_camera);
        }

        draw_screen();

        delay(1000 / 60);
    }
    return 0;
}
