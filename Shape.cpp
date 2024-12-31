#include "cygame.h"

Shape::Shape() {
    vertex_count = 0;
    index_count = 0;
    vertices = NULL;
    indices = NULL;
    num_floats_per_vertex = 0;
}

GLsizeiptr Shape::get_size_bytes() {
    return vertex_count * num_floats_per_vertex * sizeof(GLfloat);
}
GLsizeiptr Shape::get_indices_size_bytes() {
    return index_count * sizeof(GLushort);
}
GLsizeiptr Shape::get_stride_bytes() {
    return num_floats_per_vertex * sizeof(GLfloat);
}
GLsizeiptr Shape::get_color_offset() { return (3) * sizeof(GLfloat); }
GLsizeiptr Shape::get_normal_offset() { return (6) * sizeof(GLfloat); }
