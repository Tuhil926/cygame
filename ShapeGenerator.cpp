#include "cygame.h"

Shape *ShapeGenerator::get_cube() {
    Shape *ret = new Shape();
    ret->vertex_count = 24;
    ret->index_count = 36;
    ret->num_floats_per_vertex = NUM_FLOATS_PER_VERTEX;
    GLfloat verts[] = {
        1.0,       1.0,  1.0,  // top face
        /**/ 1.0,  1.0,  0.0,  //
        /**/ 0.0,  0.0,  1.0,  //
        -1.0,      1.0,  1.0,  //
        /**/ 1.0,  0.0,  1.0,  //
        /**/ 0.0,  0.0,  1.0,  //
        -1.0,      -1.0, 1.0,  //
        /**/ 0.0,  0.0,  1.0,  //
        /**/ 0.0,  0.0,  1.0,  //
        1.0,       -1.0, 1.0,  //
        /**/ 1.0,  0.0,  0.0,  //
        /**/ 0.0,  0.0,  1.0,  //
        1.0,       1.0,  -1.0, // bottom face
        /**/ 0.0,  1.0,  1.0,  //
        /**/ 0.0,  0.0,  -1.0, //
        -1.0,      1.0,  -1.0, //
        /**/ 0.0,  1.0,  0.0,  //
        /**/ 0.0,  0.0,  -1.0, //
        -1.0,      -1.0, -1.0, //
        /**/ 0.0,  1.0,  0.5,  //
        /**/ 0.0,  0.0,  -1.0, //
        1.0,       -1.0, -1.0, //
        /**/ 0.5,  1.0,  0.5,  //
        /**/ 0.0,  0.0,  -1.0, //
        1.0,       1.0,  1.0,  // right face
        /**/ 1.0,  0.0,  0.5,  //
        /**/ 1.0,  0.0,  0.0,  //
        1.0,       -1.0, 1.0,  //
        /**/ 1.0,  0.0,  1.0,  //
        /**/ 1.0,  0.0,  0.0,  //
        1.0,       -1.0, -1.0, //
        /**/ 0.0,  0.0,  1.0,  //
        /**/ 1.0,  0.0,  0.0,  //
        1.0,       1.0,  -1.0, //
        /**/ 0.0,  0.5,  1.0,  //
        /**/ 1.0,  0.0,  0.0,  //
        -1.0,      1.0,  1.0,  // left face
        /**/ 1.0,  0.5,  1.0,  //
        /**/ -1.0, 0.0,  0.0,  //
        -1.0,      -1.0, 1.0,  //
        /**/ 1.0,  0.5,  0.5,  //
        /**/ -1.0, 0.0,  0.0,  //
        -1.0,      -1.0, -1.0, //
        /**/ 0.5,  0.5,  0.5,  //
        /**/ -1.0, 0.0,  0.0,  //
        -1.0,      1.0,  -1.0, //
        /**/ 0.5,  1.0,  0.5,  //
        /**/ -1.0, 0.0,  0.0,  //
        1.0,       1.0,  1.0,  // front face
        /**/ 0.5,  1.0,  1.0,  //
        /**/ 0.0,  1.0,  0.0,  //
        -1.0,      1.0,  1.0,  //
        /**/ 0.0,  0.0,  1.0,  //
        /**/ 0.0,  1.0,  0.0,  //
        -1.0,      1.0,  -1.0, //
        /**/ 0.0,  1.0,  0.0,  //
        /**/ 0.0,  1.0,  0.0,  //
        1.0,       1.0,  -1.0, //
        /**/ 1.0,  1.0,  0.0,  //
        /**/ 0.0,  1.0,  0.0,  //
        1.0,       -1.0, 1.0,  // back face
        /**/ 1.0,  0.0,  0.0,  //
        /**/ 0.0,  -1.0, 0.0,  //
        -1.0,      -1.0, 1.0,  //
        /**/ 1.0,  0.0,  1.0,  //
        /**/ 0.0,  -1.0, 0.0,  //
        -1.0,      -1.0, -1.0, //
        /**/ 1.0,  1.0,  0.0,  //
        /**/ 0.0,  -1.0, 0.0,  //
        1.0,       -1.0, -1.0, //
        /**/ 0.0,  1.0,  0.0,  //
        /**/ 0.0,  -1.0, 0.0,  //
    };
    GLushort inds[] = {
        0,  1,  2,  2,  3,  0,  //
        4,  6,  5,  6,  4,  7,  //
        8,  9,  10, 10, 11, 8,  //
        12, 14, 13, 14, 12, 15, //
        16, 18, 17, 18, 16, 19, //
        20, 21, 22, 22, 23, 20  //
    };
    ret->vertices = (GLfloat *)malloc(ret->get_size_bytes());
    ret->indices = (GLushort *)malloc(ret->get_indices_size_bytes());
    memcpy(ret->vertices, verts, ret->get_size_bytes());
    memcpy(ret->indices, inds, ret->get_indices_size_bytes());
    return ret;
}

Shape *ShapeGenerator::get_sphere(int num_verts_in_circle, int num_circles) {
    Shape *ret = new Shape();
    ret->vertex_count = 2 + num_verts_in_circle * num_circles;
    ret->index_count = (num_circles - 1) * num_verts_in_circle * 6 +
                       2 * num_verts_in_circle * 3;
    ret->num_floats_per_vertex = NUM_FLOATS_PER_VERTEX;
    // GLfloat verts[] = {
    //     -1.0f, 1.0f,  0.0f,  // vertex1
    //     1.0f,  0.0f,  0.0f,  // color1
    //     1.0f,  1.0f,  0.0f,  // vertex2
    //     0.0f,  1.0f,  0.0f,  // color2
    //     0.0f,  -1.0f, -1.0f, // vertex3
    //     0.0f,  0.0f,  1.0f   // color3
    // };
    // GLushort inds[] = {0, 1, 2};
    std::vector<GLfloat> verts;
    std::vector<GLushort> inds;
    float angle = -glm::half_pi<float>();
    for (int i = 0; i < num_circles; i++) {
        glm::vec4 vert(1.0f, 0.0f, 0.0f, 0.0f);
        angle += glm::pi<float>() / (float)(num_circles + 1);
        auto rotate_mat = glm::rotate(glm::mat4(1), angle, {0.0f, 0.0f, 1.0f});
        vert = rotate_mat * vert;
        float y_angle = 0.0f;
        for (int j = 0; j < num_verts_in_circle; j++) {
            auto rotate_mat_y =
                glm::rotate(glm::mat4(1), y_angle, {0.0f, 1.0f, 0.0f});
            auto new_vert = rotate_mat_y * vert;
            // position
            verts.push_back(new_vert.x);
            verts.push_back(new_vert.y);
            verts.push_back(new_vert.z);

            // color
            verts.push_back(1.0);
            verts.push_back(1.0);
            verts.push_back(1.0);

            // normal
            verts.push_back(new_vert.x);
            verts.push_back(new_vert.y);
            verts.push_back(new_vert.z);

            y_angle += glm::two_pi<float>() / (float)num_verts_in_circle;
        }
    }
    // top vertex
    verts.push_back(0.0);
    verts.push_back(1.0);
    verts.push_back(0.0);

    verts.push_back(1.0);
    verts.push_back(1.0);
    verts.push_back(1.0);

    verts.push_back(0.0);
    verts.push_back(1.0);
    verts.push_back(0.0);

    // bottom vertex
    verts.push_back(0.0);
    verts.push_back(-1.0);
    verts.push_back(0.0);

    verts.push_back(1.0);
    verts.push_back(1.0);
    verts.push_back(1.0);

    verts.push_back(0.0);
    verts.push_back(-1.0);
    verts.push_back(0.0);

    for (int circle_ind = 0; circle_ind < num_circles - 1; circle_ind++) {
        for (int vert_ind = 0; vert_ind < num_verts_in_circle; vert_ind++) {
            int actual_vert_ind1 = circle_ind * num_verts_in_circle + vert_ind;
            int actual_vert_ind2 = circle_ind * num_verts_in_circle +
                                   (vert_ind + 1) % num_verts_in_circle;
            int actual_vert_ind3 = (circle_ind + 1) * num_verts_in_circle +
                                   (vert_ind + 1) % num_verts_in_circle;
            int actual_vert_ind4 =
                (circle_ind + 1) * num_verts_in_circle + vert_ind;
            inds.push_back(actual_vert_ind1);
            inds.push_back(actual_vert_ind2);
            inds.push_back(actual_vert_ind3);
            inds.push_back(actual_vert_ind1);
            inds.push_back(actual_vert_ind3);
            inds.push_back(actual_vert_ind4);
        }
    }
    for (int vert_ind = 0; vert_ind < num_verts_in_circle; vert_ind++) {
        int top_vert_ind = num_verts_in_circle * num_circles;
        int actual_vert_ind_top1 =
            (num_circles - 1) * num_verts_in_circle + vert_ind;
        int actual_vert_ind_top2 = (num_circles - 1) * num_verts_in_circle +
                                   (vert_ind + 1) % num_verts_in_circle;
        inds.push_back(actual_vert_ind_top1);
        inds.push_back(actual_vert_ind_top2);
        inds.push_back(top_vert_ind);

        int bottom_vert_ind = num_verts_in_circle * num_circles + 1;
        int actual_vert_ind_bottom1 = vert_ind;
        int actual_vert_ind_bottom2 = (vert_ind + 1) % num_verts_in_circle;
        inds.push_back(actual_vert_ind_bottom2);
        inds.push_back(actual_vert_ind_bottom1);
        inds.push_back(bottom_vert_ind);
    }
    assert(verts.size() == ret->vertex_count * NUM_FLOATS_PER_VERTEX);
    assert(inds.size() == ret->index_count);
    ret->vertices = (GLfloat *)malloc(ret->get_size_bytes());
    ret->indices = (GLushort *)malloc(ret->get_indices_size_bytes());
    memcpy(ret->vertices, verts.data(), ret->get_size_bytes());
    memcpy(ret->indices, inds.data(), ret->get_indices_size_bytes());
    return ret;
}

// don't use this, these don't have normals, and i'm too lazy to add them lol
Shape *ShapeGenerator::get_triangle() {
    Shape *ret = new Shape();
    ret->vertex_count = 3;
    ret->index_count = 3;
    ret->num_floats_per_vertex = 6;
    GLfloat verts[] = {
        0.0f,  1.0f,  -1.0f, // vertex1
        1.0f,  0.0f,  0.0f,  // color1
        1.0f,  -1.0f, 0.0f,  // vertex2
        0.0f,  1.0f,  0.0f,  // color2
        -1.0f, -1.0f, 0.0f,  // vertex3
        0.0f,  0.0f,  1.0f   // color3
    };
    GLushort inds[] = {0, 1, 2};
    ret->vertices = (GLfloat *)malloc(ret->get_size_bytes());
    ret->indices = (GLushort *)malloc(ret->get_indices_size_bytes());
    memcpy(ret->vertices, verts, ret->get_size_bytes());
    memcpy(ret->indices, inds, ret->get_indices_size_bytes());
    return ret;
}
// don't use this, these don't have normals, and i'm too lazy to add them lol
Shape *ShapeGenerator::get_triangle2() {
    Shape *ret = new Shape();
    ret->vertex_count = 3;
    ret->index_count = 3;
    ret->num_floats_per_vertex = 6;
    GLfloat verts[] = {
        -1.0f, 1.0f,  0.0f,  // vertex1
        1.0f,  0.0f,  0.0f,  // color1
        1.0f,  1.0f,  0.0f,  // vertex2
        0.0f,  1.0f,  0.0f,  // color2
        0.0f,  -1.0f, -1.0f, // vertex3
        0.0f,  0.0f,  1.0f   // color3
    };
    GLushort inds[] = {0, 1, 2};
    ret->vertices = (GLfloat *)malloc(ret->get_size_bytes());
    ret->indices = (GLushort *)malloc(ret->get_indices_size_bytes());
    memcpy(ret->vertices, verts, ret->get_size_bytes());
    memcpy(ret->indices, inds, ret->get_indices_size_bytes());
    return ret;
}
