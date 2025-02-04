#include "cygame.h"
#include "default_shaders.cpp"
#include "glad/glad.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include <iostream>
#include <iterator>
#include <string>

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
    std::string vertexShaderSrc;
    std::string fragmentShaderSrc;
    std::ifstream vertexShaderFile(vertex_shader_src_file_name);
    if (!vertexShaderFile.is_open()) {
        std::cerr << "Error: Unable to open vertex shader file, switching to "
                     "default vertex shader"
                  << std::endl;
        vertexShaderSrc = default_vertex_shader;
    } else
        vertexShaderSrc =
            std::string((std::istreambuf_iterator<char>(vertexShaderFile)),
                        std::istreambuf_iterator<char>());
    std::ifstream fragmentShaderFile(fragment_shader_src_file_name);
    if (!fragmentShaderFile.is_open()) {
        std::cerr << "Error: Unable to open fragment shader file, switching to "
                     "default fragment shader"
                  << std::endl;
        fragmentShaderSrc = default_fragment_shader;
    } else
        fragmentShaderSrc =
            std::string((std::istreambuf_iterator<char>(fragmentShaderFile)),
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

    programObject = glCreateProgram();

    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);
    glLinkProgram(programObject);

    GLint linkStatus;
    glGetProgramiv(programObject, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE) {
        GLint infoLength;
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLength);
        GLchar *buffer = new GLchar[infoLength];

        GLsizei bufferSize;
        glGetProgramInfoLog(programObject, infoLength, &bufferSize, buffer);
        std::cerr << buffer << std::endl;
        delete[] buffer;
    }
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
    spec_multiplier_uniform_location =
        glGetUniformLocation(programObject, "specMultiplier");
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

void Camera::send_uniforms(glm::mat4 &full_transform_matrix,
                           glm::mat4 &model_rotation_matrix,
                           glm::mat4 &model_to_world_matrix,
                           glm::vec3 &light_direction,
                           glm::vec3 &camera_position) {
    glUniformMatrix4fv(full_transform_matrix_location, 1, GL_FALSE,
                       &full_transform_matrix[0][0]);
    glUniformMatrix4fv(rotation_matrix_uniform_location, 1, GL_FALSE,
                       &model_rotation_matrix[0][0]);
    glUniformMatrix4fv(model_to_world_matrix_uniform_location, 1, GL_FALSE,
                       &model_to_world_matrix[0][0]);
    glUniform3fv(light_direction_uniform_location, 1, &light_direction[0]);
    glUniform3fv(camera_location_uniform_location, 1, &camera_position[0]);
}

void Camera::draw_shape(glm::mat4 model_translation_matrix,
                        glm::mat4 model_scale_matrix,
                        glm::mat4 model_rotation_matrix, ShapeOnGPU shape_gpu,
                        glm::vec3 light_direction, bool is_2D) {
    glm::mat4 full_transform_matrix;
    if (is_2D)
        full_transform_matrix =
            glm::ortho(0.0f, get_global_width(), get_global_height(), 0.0f) *
            model_translation_matrix * model_rotation_matrix *
            model_scale_matrix;
    else
        full_transform_matrix = get_world_to_perspective_transform_matrix() *
                                model_translation_matrix *
                                model_rotation_matrix * model_scale_matrix;
    glm::mat4 model_to_world_matrix =
        model_translation_matrix * model_rotation_matrix * model_scale_matrix;
    light_direction = glm::normalize(light_direction);
    send_uniforms(full_transform_matrix, model_rotation_matrix,
                  model_to_world_matrix, light_direction, position);
    ShapeRenderer::render_shape(shape_gpu);
}

void Camera::draw_2D_shape(ShapeOnGPU shape_on_gpu) {
    glDisable(GL_DEPTH_TEST);
    glUniform1f(spec_multiplier_uniform_location, 0);
    ShapeRenderer::render_shape(shape_on_gpu);
    glUniform1f(spec_multiplier_uniform_location, 1);
    glEnable(GL_DEPTH_TEST);
}

void Camera::draw_circle(Pos2D pos, float radius, Color color) {
    glm::mat4 translation_matrix =
        glm::translate(glm::mat4(1.0), glm::vec3(pos.x, pos.y, 0));
    glm::mat4 scale_matrix =
        glm::scale(glm::mat4(1.0), glm::vec3(radius, radius, 1.0));
    glm::mat4 rotation_matrix(1.0);
    glUniform1f(spec_multiplier_uniform_location, 0);
    draw_shape(translation_matrix, scale_matrix, rotation_matrix,
               get_default_circle(), {0.0f, 0.0f, 1.0f}, true);
    glUniform1f(spec_multiplier_uniform_location, 1);
}

void Camera::send_data_to_GPU(ShapeOnGPU shape_on_gpu, GLfloat verts[]) {
    glBufferSubData(
        GL_ARRAY_BUFFER,
        shape_on_gpu.offset_vertices * NUM_FLOATS_PER_VERTEX * sizeof(GLfloat),
        shape_on_gpu.num_vertices * NUM_FLOATS_PER_VERTEX * sizeof(GLfloat),
        verts);
}

void Camera::draw_triangle(Pos2D point1, Pos2D point2, Pos2D point3,
                           Color color) {
    float r = color.r / 255.0, g = color.g / 255.0, b = color.b / 255.0;
    GLfloat verts[] = {
        point1.x, point1.y, 1.0, //
        /**/ r,   g,        b,   //
        /**/ 0.0, 0.0,      1.0, //
        /**/ 1.0, 1.0,           //
        point2.x, point2.y, 1.0, //
        /**/ r,   g,        b,   //
        /**/ 0.0, 0.0,      1.0, //
        /**/ 0.0, 1.0,           //
        point3.x, point3.y, 1.0, //
        /**/ r,   g,        b,   //
        /**/ 0.0, 0.0,      1.0, //
        /**/ 0.0, 0.0,           //
    };
    glm::mat4 projection_matrix =
        glm::ortho(0.0f, get_global_width(), get_global_height(), 0.0f);
    glm::mat4 rotation_matrix(1.0f);
    glm::vec3 light_direction(0.0f, 0.0f, 1.0f);
    glm::vec3 position_2d(0.0f, 0.0f, 1.0f);
    send_uniforms(projection_matrix, rotation_matrix, projection_matrix,
                  light_direction, position_2d);
    ShapeOnGPU default_rect = get_default_rect();
    default_rect.num_indices = 3;
    default_rect.num_vertices = 3;
    send_data_to_GPU(default_rect, verts);

    draw_2D_shape(default_rect);
}

void Camera::draw_quad(Pos2D point1, Pos2D point2, Pos2D point3, Pos2D point4,
                       Color color) {
    float r = color.r / 255.0, g = color.g / 255.0, b = color.b / 255.0;
    GLfloat verts[] = {
        point1.x, point1.y, 1.0, //
        /**/ r,   g,        b,   //
        /**/ 0.0, 0.0,      1.0, //
        /**/ 1.0, 1.0,           //
        point2.x, point2.y, 1.0, //
        /**/ r,   g,        b,   //
        /**/ 0.0, 0.0,      1.0, //
        /**/ 0.0, 1.0,           //
        point3.x, point3.y, 1.0, //
        /**/ r,   g,        b,   //
        /**/ 0.0, 0.0,      1.0, //
        /**/ 0.0, 0.0,           //
        point4.x, point4.y, 1.0, //
        /**/ r,   g,        b,   //
        /**/ 0.0, 0.0,      1.0, //
        /**/ 1.0, 0.0,           //
    };
    glm::mat4 projection_matrix =
        glm::ortho(0.0f, get_global_width(), get_global_height(), 0.0f);
    glm::mat4 rotation_matrix(1.0f);
    glm::vec3 light_direction(0.0f, 0.0f, 1.0f);
    glm::vec3 position_2d(0.0f, 0.0f, 1.0f);
    send_uniforms(projection_matrix, rotation_matrix, projection_matrix,
                  light_direction, position_2d);
    ShapeOnGPU default_rect = get_default_rect();
    send_data_to_GPU(default_rect, verts);

    draw_2D_shape(default_rect);
}
void Camera::draw_gradient_quad(Pos2D point1, Pos2D point2, Pos2D point3,
                                Pos2D point4, Color color1, Color color2,
                                Color color3, Color color4) {

    float r1 = color1.r / 255.0, g1 = color1.g / 255.0, b1 = color1.b / 255.0;
    float r2 = color2.r / 255.0, g2 = color2.g / 255.0, b2 = color2.b / 255.0;
    float r3 = color3.r / 255.0, g3 = color3.g / 255.0, b3 = color3.b / 255.0;
    float r4 = color4.r / 255.0, g4 = color4.g / 255.0, b4 = color4.b / 255.0;
    GLfloat verts[] = {
        point1.x, point1.y, 1.0, //
        /**/ r1,  g1,       b1,  //
        /**/ 0.0, 0.0,      1.0, //
        /**/ 1.0, 1.0,           //
        point2.x, point2.y, 1.0, //
        /**/ r2,  g2,       b2,  //
        /**/ 0.0, 0.0,      1.0, //
        /**/ 0.0, 1.0,           //
        point3.x, point3.y, 1.0, //
        /**/ r3,  g3,       b3,  //
        /**/ 0.0, 0.0,      1.0, //
        /**/ 0.0, 0.0,           //
        point4.x, point4.y, 1.0, //
        /**/ r4,  g4,       b4,  //
        /**/ 0.0, 0.0,      1.0, //
        /**/ 1.0, 0.0,           //
    };
    glm::mat4 projection_matrix =
        glm::ortho(0.0f, get_global_width(), get_global_height(), 0.0f);
    glm::mat4 rotation_matrix(1.0f);
    glm::vec3 light_direction(0.0f, 0.0f, 1.0f);
    glm::vec3 position_2d(0.0f, 0.0f, 1.0f);
    send_uniforms(projection_matrix, rotation_matrix, projection_matrix,
                  light_direction, position_2d);
    ShapeOnGPU default_rect = get_default_rect();
    send_data_to_GPU(default_rect, verts);

    draw_2D_shape(default_rect);
}

void Camera::draw_rect(SDL_Rect rect, Color color) {
    float xpos = rect.x, ypos = rect.y, w = rect.w, h = rect.h;
    draw_quad({xpos + w, ypos + h}, {xpos, ypos + h}, {xpos, ypos},
              {xpos + w, ypos}, color);
}

void Camera::draw_line(Pos2D pos1, Pos2D pos2, Color color, float width) {

    Pos2D dir = (pos2 - pos1);
    Pos2D normal = dir * width / dir.norm();
    Pos2D perpendicular = {-normal.y, normal.x};
    draw_quad(pos1 + perpendicular, pos1 - perpendicular, pos2 - perpendicular,
              pos2 + perpendicular, color);
}

void Camera::render_text(std::string text, float x, float y, glm::vec3 color,
                         Font *font) {
    float scale = 1.0;
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(is_texture_uniform_location, 2);
    glUniform1f(spec_multiplier_uniform_location, 0);
    glm::mat4 projection_matrix =
        glm::ortho(0.0f, get_global_width(), get_global_height(), 0.0f);
    glm::mat4 rotation_matrix(1.0f);
    glm::vec3 light_direction(0.0f, 0.0f, 1.0f);
    glm::vec3 position_2d(0.0f, 0.0f, 1.0f);
    send_uniforms(projection_matrix, rotation_matrix, projection_matrix,
                  light_direction, position_2d);
    glDisable(GL_DEPTH_TEST);
    for (char c : text) {
        Character character = font->characters[c];
        float xpos = x + character.bearing.x * scale;
        float ypos = y - character.bearing.y * scale;

        float w = character.size.x * scale;
        float h = character.size.y * scale;

        GLfloat verts[] = {
            xpos + w,     ypos + h, 1.0,     //
            /**/ color.r, color.g,  color.b, //
            /**/ 0.0,     0.0,      1.0,     //
            /**/ 1.0,     1.0,               //
            xpos,         ypos + h, 1.0,     //
            /**/ color.r, color.g,  color.b, //
            /**/ 0.0,     0.0,      1.0,     //
            /**/ 0.0,     1.0,               //
            xpos,         ypos,     1.0,     //
            /**/ color.r, color.g,  color.b, //
            /**/ 0.0,     0.0,      1.0,     //
            /**/ 0.0,     0.0,               //
            xpos + w,     ypos,     1.0,     //
            /**/ color.r, color.g,  color.b, //
            /**/ 0.0,     0.0,      1.0,     //
            /**/ 1.0,     0.0,               //
        };
        ShapeOnGPU default_rect = get_default_rect();
        glBindTexture(GL_TEXTURE_2D, character.TextureID);
        send_data_to_GPU(default_rect, verts);

        x += (character.advance >> 6) * scale;
        ShapeRenderer::render_shape(default_rect);
    }
    glEnable(GL_DEPTH_TEST);
    glUniform1f(spec_multiplier_uniform_location, 1);
    glUniform1i(is_texture_uniform_location, 0);
}

void Camera::draw_image_in_quad(Image &image, Pos2D point1, Pos2D point2,
                                Pos2D point3, Pos2D point4) {
    glUniform1i(is_texture_uniform_location, 1);
    glBindTexture(GL_TEXTURE_2D, image.texture_id);
    draw_quad(point1, point2, point3, point4, {255, 255, 255, 255});
    glUniform1i(is_texture_uniform_location, 0);
}

void Camera::draw_image(Image &image, SDL_Rect rect) {
    float xpos = rect.x, ypos = rect.y, w = rect.w, h = rect.h;
    draw_image_in_quad(image, {xpos + w, ypos + h}, {xpos, ypos + h},
                       {xpos, ypos}, {xpos + w, ypos});
}
