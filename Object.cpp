#include "cygame.h"

Object::Object(ShapeOnGPU shape_on_gpu) {
    this->shape_on_gpu = shape_on_gpu;
    position = glm::vec3(0.0f, 0.0f, -3.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
    rotation = glm::mat4(1.0f);
}
// glm::mat4 Object::get_model_to_world_transform_matrix() {
//     glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position);
//     glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), scale);
//     return translation_matrix * scale_matrix;
// }
glm::mat4 Object::get_model_to_world_translation_matrix() {
    return glm::translate(glm::mat4(1.0f), position);
}
glm::mat4 Object::get_model_to_world_scale_matrix() {
    return glm::scale(glm::mat4(1.0f), scale);
}
glm::mat4 Object::get_model_to_world_rotation_matrix() { return rotation; }
void Object::draw(Camera &camera) {
    camera.draw_shape(get_model_to_world_translation_matrix(),
                      get_model_to_world_scale_matrix(),
                      get_model_to_world_rotation_matrix(), this->shape_on_gpu);
}
void Object::rotate_x(float angle) {
    rotation = glm::rotate(rotation, angle, glm::vec3(1.0f, 0.0f, 0.0f));
}
void Object::rotate_y(float angle) {
    rotation = glm::rotate(rotation, angle, glm::vec3(0.0f, 1.0f, 0.0f));
}
void Object::rotate_z(float angle) {
    rotation = glm::rotate(rotation, angle, glm::vec3(0.0f, 0.0f, 1.0f));
}
