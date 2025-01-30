#include "cygame.h"
#include "glad/glad.h"
#include "glm/ext/matrix_clip_space.hpp"
#include <iostream>
#include <iterator>

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
    rotation_matrix_uniform_location =
        glGetUniformLocation(programObject, "rotationMatrix");
    camera_location_uniform_location =
        glGetUniformLocation(programObject, "cameraLocation");
    model_to_world_matrix_uniform_location =
        glGetUniformLocation(programObject, "modelToWorldMatrix");
    text_uniform_location = glGetUniformLocation(programObject, "text");
    is_texture_uniform_location =
        glGetUniformLocation(programObject, "isTexture");
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
void Camera::draw_shape(glm::mat4 model_translation_matrix,
                        glm::mat4 model_scale_matrix,
                        glm::mat4 model_rotation_matrix, ShapeOnGPU shape_gpu) {
    glm::mat4 full_transform_matrix =
        get_world_to_perspective_transform_matrix() * model_translation_matrix *
        model_rotation_matrix * model_scale_matrix;
    glm::mat4 model_to_world_matrix =
        model_translation_matrix * model_rotation_matrix * model_scale_matrix;
    // glm::vec3 light_direction(1.0f, 3.0f, 2.0f);
    glm::vec3 light_direction(1.0f, 3.0f, 2.0f);
    light_direction = glm::normalize(light_direction);
    glUniformMatrix4fv(full_transform_matrix_location, 1, GL_FALSE,
                       &full_transform_matrix[0][0]);
    glUniformMatrix4fv(rotation_matrix_uniform_location, 1, GL_FALSE,
                       &model_rotation_matrix[0][0]);
    glUniformMatrix4fv(model_to_world_matrix_uniform_location, 1, GL_FALSE,
                       &model_to_world_matrix[0][0]);
    glUniform3fv(light_direction_uniform_location, 1, &light_direction[0]);
    glUniform3fv(camera_location_uniform_location, 1, &position[0]);
    ShapeRenderer::render_shape(shape_gpu);
}

void Camera::render_text(std::string text, float x, float y, glm::vec3 color,
                         Font *font) {
    float scale = 1.0;
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(is_texture_uniform_location, 1);
    glm::mat4 projection_matrix =
        glm::ortho(0.0f, get_global_width(), get_global_height(), 0.0f);
    glm::mat4 rotation_matrix(1.0f);
    glm::vec3 light_direction(0.0f, 0.0f, 1.0f);
    glm::vec3 position_2d(0.0f, 0.0f, 1.0f);
    glUniformMatrix4fv(full_transform_matrix_location, 1, GL_FALSE,
                       &projection_matrix[0][0]);
    glUniformMatrix4fv(rotation_matrix_uniform_location, 1, GL_FALSE,
                       &rotation_matrix[0][0]);
    glUniformMatrix4fv(model_to_world_matrix_uniform_location, 1, GL_FALSE,
                       &projection_matrix[0][0]);
    glUniform3fv(light_direction_uniform_location, 1, &light_direction[0]);
    glUniform3fv(camera_location_uniform_location, 1, &position[0]);
    for (char c : text) {
        Character character = font->characters[c];
        float xpos = x + character.bearing.x * scale;
        float ypos = y - character.bearing.y * scale;

        float w = character.size.x * scale;
        float h = character.size.y * scale;

        // std::cout << xpos << " " << ypos << " " << w << ' ' << h <<
        // std::endl;

        GLfloat verts[] = {
            xpos + w,     ypos + h, 0.0,     //
            /**/ color.r, color.g,  color.b, //
            /**/ 0.0,     0.0,      1.0,     //
            /**/ 1.0,     1.0,               //
            xpos,         ypos + h, 0.0,     // top face
            /**/ color.r, color.g,  color.b, //
            /**/ 0.0,     0.0,      1.0,     //
            /**/ 0.0,     1.0,               //
            xpos,         ypos,     0.0,     //
            /**/ color.r, color.g,  color.b, //
            /**/ 0.0,     0.0,      1.0,     //
            /**/ 0.0,     0.0,               //
            xpos + w,     ypos,     0.0,     //
            /**/ color.r, color.g,  color.b, //
            /**/ 0.0,     0.0,      1.0,     //
            /**/ 1.0,     0.0,               //
        };
        // GLushort inds[] = {0, 1, 2, 2, 3, 0};
        ShapeOnGPU default_rect = get_default_rect();
        glBindTexture(GL_TEXTURE_2D, character.TextureID);
        glBufferSubData(GL_ARRAY_BUFFER,
                        default_rect.offset_vertices * NUM_FLOATS_PER_VERTEX *
                            sizeof(GLfloat),
                        default_rect.num_vertices * NUM_FLOATS_PER_VERTEX *
                            sizeof(GLfloat),
                        verts);

        x += (character.advance >> 6) * scale;
        ShapeRenderer::render_shape(default_rect);
    }
    glUniform1i(is_texture_uniform_location, 0);
}
