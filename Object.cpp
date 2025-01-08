#include "cygame.h"

Object::Object(ShapeOnGPU shape_on_gpu) {
    this->shape_on_gpu = shape_on_gpu;
    position = glm::vec3(0.0f, 0.0f, -3.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
}
glm::mat4 Object::get_model_to_world_transform_matrix() {
    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), scale);
    return translation_matrix * scale_matrix;
}
void Object::draw(Camera &camera) {
    camera.draw_shape(get_model_to_world_transform_matrix(),
                      this->shape_on_gpu);
}
