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
GLsizeiptr Shape::get_texture_coord_offset() { return (9) * sizeof(GLfloat); }
void Shape::print_values() {
    for (int i = 0; i < vertex_count; i++) {
        std::cout << "vert: " << vertices[NUM_FLOATS_PER_VERTEX * i + 0] << ' '
                  << vertices[NUM_FLOATS_PER_VERTEX * i + 1] << ' '
                  << vertices[NUM_FLOATS_PER_VERTEX * i + 2] << std::endl;
        std::cout << "colr: " << vertices[NUM_FLOATS_PER_VERTEX * i + 3] << ' '
                  << vertices[NUM_FLOATS_PER_VERTEX * i + 4] << ' '
                  << vertices[NUM_FLOATS_PER_VERTEX * i + 5] << std::endl;
        std::cout << "norm: " << vertices[NUM_FLOATS_PER_VERTEX * i + 6] << ' '
                  << vertices[NUM_FLOATS_PER_VERTEX * i + 7] << ' '
                  << vertices[NUM_FLOATS_PER_VERTEX * i + 8] << std::endl;
        std::cout << std::endl;
    }
    for (int i = 0; i < index_count; i++) {
        std::cout << indices[i] << ' ';
        if (i % 3 == 2)
            std::cout << std::endl;
    }
}

ShapeOnGPU::ShapeOnGPU() {
    num_vertices = 0;
    num_indices = 0;
}
