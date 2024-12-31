#include "cygame.h"

Camera::Camera() {
    position = glm::vec3(0, 0, 0);
    up = glm::vec3(0, 1, 0);
    front = glm::vec3(0, 0, -1);
    fov = glm::pi<float>() / 3;
    right = glm::cross(front, up);
}

Camera::Camera(std::string vertex_shader_src_file_name,
               std::string fragment_shader_src_file_name)
    : Camera() {
    std::ifstream vertexShaderFile(vertex_shader_src_file_name);
    if (!vertexShaderFile.is_open()) {
        std::cerr << "Error: Unable to open vertex shader file" << std::endl;
    }
    std::string vertexShaderSrc(
        (std::istreambuf_iterator<char>(vertexShaderFile)),
        std::istreambuf_iterator<char>());
    std::ifstream fragmentShaderFile(fragment_shader_src_file_name);
    if (!fragmentShaderFile.is_open()) {
        std::cerr << "Error: Unable to open fragment shader file" << std::endl;
    }
    std::string fragmentShaderSrc(
        (std::istreambuf_iterator<char>(fragmentShaderFile)),
        std::istreambuf_iterator<char>());

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
        std::cerr << buffer << std::endl;
        delete[] buffer;
    }
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE) {
        GLint infoLength;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLength);
        GLchar *buffer = new GLchar[infoLength];

        GLsizei bufferSize;
        glGetShaderInfoLog(fragmentShader, infoLength, &bufferSize, buffer);
        std::cerr << buffer << std::endl;
        delete[] buffer;
    }

    GLuint programObject = glCreateProgram();

    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);
    glLinkProgram(programObject);
    glValidateProgram(programObject);

    glEnable(GL_DEPTH_TEST);
    // glDisable(GL_CULL_FACE);
    glEnable(GL_CULL_FACE);
    glUseProgram(programObject);

    full_transform_matrix_location =
        glGetUniformLocation(programObject, "fullTransformMatrix");
    light_direction_uniform_location =
        glGetUniformLocation(programObject, "lightDirection");
}
glm::mat4 Camera::get_world_to_perspective_transform_matrix() {
    glm::vec3 looking_vector =
        glm::vec3(glm::rotate(glm::mat4(1), looking_angle, right) *
                  glm::vec4(front, 1.0));
    return glm::perspective(fov, get_global_aspect_ratio(), near_plane,
                            far_plane) *
           glm::lookAt(position, position + looking_vector, up);
}
void Camera::rotate_left_right(float angle) {
    front =
        glm::vec3(glm::rotate(glm::mat4(1), angle, up) * glm::vec4(front, 1.0));
    right = glm::cross(front, up);
}
void Camera::rotate_up_down(float angle) {
    looking_angle += angle;
    looking_angle = glm::clamp(looking_angle, -glm::half_pi<float>() + 0.00001f,
                               glm::half_pi<float>() - 0.00001f);
}
void Camera::move_front(float distance) { position += front * distance; }
void Camera::strafe(float distance) { position += right * distance; }
void Camera::move_up(float distance) { position += up * distance; }
void Camera::track_input(Keys keys, MouseState mouse_state, float dt) {
    float distance = movement_speed * dt;
    if (keys[K_ctrl]) {
        distance *= 3;
    }
    if (keys[K_w]) {
        move_front(distance);
    }
    if (keys[K_s]) {
        move_front(-distance);
    }
    if (keys[K_a]) {
        strafe(-distance);
    }
    if (keys[K_d]) {
        strafe(distance);
    }
    if (keys[K_shift]) {
        move_up(-distance);
    }
    if (keys[K_space]) {
        move_up(distance);
    }
    if (minecraft_rotation) {
        rotate_left_right(-(mouse_state.x - prev_mouse_state.x) * 2.0 /
                          get_global_width());
        rotate_up_down(-(mouse_state.y - prev_mouse_state.y) * 2.0 /
                       get_global_width());
        SDL_WarpMouseInWindow(get_global_window(), get_global_width() / 2,
                              get_global_height() / 2);
        prev_mouse_state.x = get_global_width() / 2;
        prev_mouse_state.y = get_global_height() / 2;
    } else {
        if (mouse_state.pressed_left()) {
            rotate_left_right((mouse_state.x - prev_mouse_state.x) * 2.0 /
                              get_global_width());
            rotate_up_down((mouse_state.y - prev_mouse_state.y) * 2.0 /
                           get_global_width());
        }
        prev_mouse_state = mouse_state;
    }
}
void Camera::toggle_minecraft_rotation() {
    if (!minecraft_rotation) {
        minecraft_rotation = true;
        SDL_ShowCursor(SDL_DISABLE);
    } else {
        minecraft_rotation = false;
        SDL_ShowCursor(SDL_ENABLE);
    }
}
void Camera::draw_shape(glm::mat4 model_transform_matrix,
                        ShapeOnGPU shape_gpu) {
    glm::mat4 full_transform_matrix =
        get_world_to_perspective_transform_matrix() * model_transform_matrix;
    glm::vec3 light_direction(1.0f, 3.0f, 2.0f);
    light_direction = glm::normalize(light_direction);
    glUniformMatrix4fv(full_transform_matrix_location, 1, GL_FALSE,
                       &full_transform_matrix[0][0]);
    glUniform3fv(light_direction_uniform_location, 1, &light_direction[0]);
    ShapeRenderer::render_shape(shape_gpu);
}
