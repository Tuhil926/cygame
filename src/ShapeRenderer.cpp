#include "cygame.h"

ShapeOnGPU default_rect;
ShapeOnGPU default_circle;

ShapeOnGPU get_default_rect() { return default_rect; }
ShapeOnGPU get_default_circle() { return default_circle; }
ShapeRenderer::ShapeRenderer() {
    Shape *rect = ShapeGenerator::get_rect();
    default_rect = this->add_shape(rect);
    Shape *circle = ShapeGenerator::get_circle(100);
    default_circle = this->add_shape(circle);
}
ShapeOnGPU ShapeRenderer::add_shape(Shape *shape) {
    ShapeOnGPU ret;
    ret.offset_vertices = tot_offset_vertices;
    ret.num_vertices = shape->vertex_count;
    ret.offset_indices = tot_offset_indices;
    ret.num_indices = shape->index_count;

    shapes.push_back(shape);
    shapes_on_gpu.push_back(ret);
    tot_offset_vertices += shape->vertex_count;
    tot_offset_indices += shape->index_count;

    return ret;
}
void ShapeRenderer::send_shapes() {
    if (sent_already) {
        std::cerr << "Cannot send shapes twice! Please add all shapes "
                     "before sending"
                  << std::endl;
        exit(1);
    }
    sent_already = true;
    if (!shapes.size())
        return;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    Shape *combined_shape = new Shape();
    combined_shape->num_floats_per_vertex = shapes[0]->num_floats_per_vertex;
    combined_shape->vertex_count = tot_offset_vertices;
    combined_shape->vertices =
        (GLfloat *)malloc(combined_shape->get_size_bytes());
    combined_shape->index_count = tot_offset_indices;
    combined_shape->indices =
        (GLuint *)malloc(combined_shape->get_indices_size_bytes());

    int tot_byte_offset_vertices = 0;
    int tot_num_indices = 0;
    int tot_num_vertices = 0;
    for (Shape *shape : shapes) {
        memcpy((char *)((uint64_t)combined_shape->vertices +
                        (uint64_t)tot_byte_offset_vertices),
               shape->vertices, shape->get_size_bytes());

        for (int i = 0; i < shape->index_count; i++) {
            combined_shape->indices[tot_num_indices + i] =
                tot_num_vertices + shape->indices[i];
        }

        tot_byte_offset_vertices += shape->get_size_bytes();
        tot_num_indices += shape->index_count;
        tot_num_vertices += shape->vertex_count;
    }
    // for (int i = 0; i < 6; i++) {
    //     std::cout << combined_shape->indices[i] << std::endl;
    // }

    // generate array buffer and send vertex data
    glGenBuffers(1, &myBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, myBufferID);
    glBufferData(GL_ARRAY_BUFFER, combined_shape->get_size_bytes(),
                 combined_shape->vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          combined_shape->get_stride_bytes(), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          combined_shape->get_stride_bytes(),
                          (void *)(combined_shape->get_color_offset()));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
                          combined_shape->get_stride_bytes(),
                          (void *)(combined_shape->get_normal_offset()));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE,
                          combined_shape->get_stride_bytes(),
                          (void *)(combined_shape->get_texture_coord_offset()));

    // generate element array buffer and send index data
    glGenBuffers(1, &myElementBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myElementBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 combined_shape->get_indices_size_bytes(),
                 combined_shape->indices, GL_STATIC_DRAW);
}
void ShapeRenderer::render_shape(ShapeOnGPU shape_gpu) {
    // glDrawArrays(GL_TRIANGLES, shape_gpu.offset_vertices,
    //              shape_gpu.num_vertices);
    // TODO: Change to GL_UNSIGNED_INT
    glDrawElements(GL_TRIANGLES, shape_gpu.num_indices, GL_UNSIGNED_INT,
                   (void *)(shape_gpu.offset_indices * sizeof(GLuint)));
}
