#include "cygame.h"
#include "glad/glad.h"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

Shape *ShapeGenerator::get_cube() {
    Shape *ret = new Shape();
    ret->vertex_count = 24;
    ret->index_count = 36;
    ret->num_floats_per_vertex = NUM_FLOATS_PER_VERTEX;
    GLfloat verts[] = {
        1.0,       1.0,  1.0,  // top face
        /**/ 1.0,  1.0,  0.0,  //
        /**/ 0.0,  0.0,  1.0,  //
        /**/ 0.0,  0.0,        //
        -1.0,      1.0,  1.0,  //
        /**/ 1.0,  0.0,  1.0,  //
        /**/ 0.0,  0.0,  1.0,  //
        /**/ 0.0,  0.0,        //
        -1.0,      -1.0, 1.0,  //
        /**/ 0.0,  0.0,  1.0,  //
        /**/ 0.0,  0.0,  1.0,  //
        /**/ 0.0,  0.0,        //
        1.0,       -1.0, 1.0,  //
        /**/ 1.0,  0.0,  0.0,  //
        /**/ 0.0,  0.0,  1.0,  //
        /**/ 0.0,  0.0,        //
        1.0,       1.0,  -1.0, // bottom face
        /**/ 0.0,  1.0,  1.0,  //
        /**/ 0.0,  0.0,  -1.0, //
        /**/ 0.0,  0.0,        //
        -1.0,      1.0,  -1.0, //
        /**/ 0.0,  1.0,  0.0,  //
        /**/ 0.0,  0.0,  -1.0, //
        /**/ 0.0,  0.0,        //
        -1.0,      -1.0, -1.0, //
        /**/ 0.0,  1.0,  0.5,  //
        /**/ 0.0,  0.0,  -1.0, //
        /**/ 0.0,  0.0,        //
        1.0,       -1.0, -1.0, //
        /**/ 0.5,  1.0,  0.5,  //
        /**/ 0.0,  0.0,  -1.0, //
        /**/ 0.0,  0.0,        //
        1.0,       1.0,  1.0,  // right face
        /**/ 1.0,  0.0,  0.5,  //
        /**/ 1.0,  0.0,  0.0,  //
        /**/ 0.0,  0.0,        //
        1.0,       -1.0, 1.0,  //
        /**/ 1.0,  0.0,  1.0,  //
        /**/ 1.0,  0.0,  0.0,  //
        /**/ 0.0,  0.0,        //
        1.0,       -1.0, -1.0, //
        /**/ 0.0,  0.0,  1.0,  //
        /**/ 1.0,  0.0,  0.0,  //
        /**/ 0.0,  0.0,        //
        1.0,       1.0,  -1.0, //
        /**/ 0.0,  0.5,  1.0,  //
        /**/ 1.0,  0.0,  0.0,  //
        /**/ 0.0,  0.0,        //
        -1.0,      1.0,  1.0,  // left face
        /**/ 1.0,  0.5,  1.0,  //
        /**/ -1.0, 0.0,  0.0,  //
        /**/ 0.0,  0.0,        //
        -1.0,      -1.0, 1.0,  //
        /**/ 1.0,  0.5,  0.5,  //
        /**/ -1.0, 0.0,  0.0,  //
        /**/ 0.0,  0.0,        //
        -1.0,      -1.0, -1.0, //
        /**/ 0.5,  0.5,  0.5,  //
        /**/ -1.0, 0.0,  0.0,  //
        /**/ 0.0,  0.0,        //
        -1.0,      1.0,  -1.0, //
        /**/ 0.5,  1.0,  0.5,  //
        /**/ -1.0, 0.0,  0.0,  //
        /**/ 0.0,  0.0,        //
        1.0,       1.0,  1.0,  // front face
        /**/ 0.5,  1.0,  1.0,  //
        /**/ 0.0,  1.0,  0.0,  //
        /**/ 0.0,  0.0,        //
        -1.0,      1.0,  1.0,  //
        /**/ 0.0,  0.0,  1.0,  //
        /**/ 0.0,  1.0,  0.0,  //
        /**/ 0.0,  0.0,        //
        -1.0,      1.0,  -1.0, //
        /**/ 0.0,  1.0,  0.0,  //
        /**/ 0.0,  1.0,  0.0,  //
        /**/ 0.0,  0.0,        //
        1.0,       1.0,  -1.0, //
        /**/ 1.0,  1.0,  0.0,  //
        /**/ 0.0,  1.0,  0.0,  //
        /**/ 0.0,  0.0,        //
        1.0,       -1.0, 1.0,  // back face
        /**/ 1.0,  0.0,  0.0,  //
        /**/ 0.0,  -1.0, 0.0,  //
        /**/ 0.0,  0.0,        //
        -1.0,      -1.0, 1.0,  //
        /**/ 1.0,  0.0,  1.0,  //
        /**/ 0.0,  -1.0, 0.0,  //
        /**/ 0.0,  0.0,        //
        -1.0,      -1.0, -1.0, //
        /**/ 1.0,  1.0,  0.0,  //
        /**/ 0.0,  -1.0, 0.0,  //
        /**/ 0.0,  0.0,        //
        1.0,       -1.0, -1.0, //
        /**/ 0.0,  1.0,  0.0,  //
        /**/ 0.0,  -1.0, 0.0,  //
        /**/ 0.0,  0.0,        //
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

Shape *ShapeGenerator::get_rect() {
    Shape *ret = new Shape();
    ret->vertex_count = 4;
    ret->index_count = 6;
    ret->num_floats_per_vertex = NUM_FLOATS_PER_VERTEX;
    float xpos = 500;
    float ypos = 50;
    float w = 9;
    float h = 13;
    GLfloat verts[] = {
        1.0,       1.0,  0.0, // top face
        /**/ 1.0,  1.0,  1.0, //
        /**/ 0.0,  0.0,  1.0, //
        /**/ 1.0,  1.0,       //
        -1.0,      1.0,  0.0, //
        /**/ 1.0,  1.0,  1.0, //
        /**/ 0.0,  0.0,  1.0, //
        /**/ -1.0, 1.0,       //
        -1.0,      -1.0, 0.0, //
        /**/ 1.0,  1.0,  1.0, //
        /**/ 0.0,  0.0,  1.0, //
        /**/ -1.0, -1.0,      //
        1.0,       -1.0, 0.0, //
        /**/ 1.0,  1.0,  1.0, //
        /**/ 0.0,  0.0,  1.0, //
        /**/ 1.0,  -1.0,      //
    };
    // GLfloat verts[] = {
    //     xpos + w,  ypos + h, 0.0, //
    //     /**/ 1.0,  1.0,      1.0, //
    //     /**/ 0.0,  0.0,      1.0, //
    //     /**/ 1.0,  1.0,           //
    //     xpos,      ypos + h, 0.0, // top face
    //     /**/ 1.0,  1.0,      1.0, //
    //     /**/ 0.0,  0.0,      1.0, //
    //     /**/ -1.0, 1.0,           //
    //     xpos,      ypos,     0.0, //
    //     /**/ 1.0,  1.0,      1.0, //
    //     /**/ 0.0,  0.0,      1.0, //
    //     /**/ -1.0, -1.0,          //
    //     xpos + w,  ypos,     0.0, //
    //     /**/ 1.0,  1.0,      1.0, //
    //     /**/ 0.0,  0.0,      1.0, //
    //     /**/ 1.0,  -1.0,          //
    // };
    GLushort inds[] = {0, 2, 1, 2, 0, 3};
    ret->vertices = (GLfloat *)malloc(ret->get_size_bytes());
    ret->indices = (GLushort *)malloc(ret->get_indices_size_bytes());
    memcpy(ret->vertices, verts, ret->get_size_bytes());
    memcpy(ret->indices, inds, ret->get_indices_size_bytes());
    return ret;
}

Shape *ShapeGenerator::get_circle(int num_vertices) {
    Shape *ret = new Shape();
    ret->vertex_count = num_vertices + 1;
    ret->index_count = 3 * num_vertices;
    ret->num_floats_per_vertex = NUM_FLOATS_PER_VERTEX;
    std::vector<GLfloat> verts;
    std::vector<GLushort> inds;
    glm::vec4 vert(1.0f, 0.0f, 0.0f, 0.0f);
    float y_angle = 0.0f;
    for (int j = 0; j < num_vertices; j++) {
        auto rotate_mat_y =
            glm::rotate(glm::mat4(1), y_angle, {0.0f, 0.0f, 1.0f});
        auto new_vert = rotate_mat_y * vert;
        // position
        verts.push_back(new_vert.x);
        verts.push_back(new_vert.y);
        verts.push_back(1.0);

        // color
        verts.push_back(1.0);
        verts.push_back(1.0);
        verts.push_back(1.0);

        // normal
        verts.push_back(0);
        verts.push_back(0);
        verts.push_back(1);

        // text_coord

        verts.push_back(0);
        verts.push_back(0);

        y_angle += glm::two_pi<float>() / (float)num_vertices;
    }
    // top vertex
    verts.push_back(0.0);
    verts.push_back(0.0);
    verts.push_back(1.0);

    verts.push_back(1.0);
    verts.push_back(1.0);
    verts.push_back(1.0);

    verts.push_back(0.0);
    verts.push_back(0.0);
    verts.push_back(1.0);

    verts.push_back(0);
    verts.push_back(0);

    for (int vert_ind = 0; vert_ind < num_vertices; vert_ind++) {
        int top_vert_ind = num_vertices;
        int actual_vert_ind_top1 = vert_ind;
        int actual_vert_ind_top2 = (vert_ind + 1) % num_vertices;
        inds.push_back(actual_vert_ind_top2);
        inds.push_back(actual_vert_ind_top1);
        inds.push_back(top_vert_ind);
    }
    assert(verts.size() == ret->vertex_count * NUM_FLOATS_PER_VERTEX);
    assert(inds.size() == ret->index_count);
    ret->vertices = (GLfloat *)malloc(ret->get_size_bytes());
    ret->indices = (GLushort *)malloc(ret->get_indices_size_bytes());
    memcpy(ret->vertices, verts.data(), ret->get_size_bytes());
    memcpy(ret->indices, inds.data(), ret->get_indices_size_bytes());
    return ret;

    return ret;
}

Shape *ShapeGenerator::get_sphere(int num_verts_in_circle, int num_circles) {
    Shape *ret = new Shape();
    ret->vertex_count = 2 + num_verts_in_circle * num_circles;
    ret->index_count = (num_circles - 1) * num_verts_in_circle * 6 +
                       2 * num_verts_in_circle * 3;
    ret->num_floats_per_vertex = NUM_FLOATS_PER_VERTEX;
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

            // text_coord

            verts.push_back(0);
            verts.push_back(0);

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

    verts.push_back(0);
    verts.push_back(0);

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

    verts.push_back(0);
    verts.push_back(0);

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

struct Material {
    glm::vec3 color;
    Material() { color = glm::vec3(1.0f, 1.0f, 1.0f); }
} typedef Material;

void read_material_file(std::string filename,
                        std::map<std::string, Material> &material_lib) {
    std::cout << "Material File name: " << filename << std::endl;
    std::ifstream inp_file(filename);
    std::string curr_material_name;
    std::string line;
    while (getline(inp_file, line)) {
        // std::cout << "line: " << line << std::endl;
        std::stringstream line_stream(line);
        std::string linetype;
        line_stream >> linetype;
        if (linetype[0] == '#')
            continue;
        else if (linetype == "newmtl") {
            line_stream >> curr_material_name;
        } else if (linetype == "Kd") {
            float r, g, b;
            line_stream >> r >> g >> b;
            material_lib[curr_material_name].color = glm::vec3(r, g, b);
        }
    }
}

Shape *ShapeGenerator::get_from_file(std::string filename) {
    Shape *ret = new Shape();
    std::ifstream inp_file(filename);
    std::vector<glm::vec3> obj_vertices;
    std::vector<glm::vec3> obj_normals;
    std::vector<glm::vec2> obj_texture_coords;
    std::vector<GLfloat> verts;
    std::vector<GLushort> indices;
    std::map<std::string, Material> materials;
    int num_verts = 0;
    Material curr_material;

    std::string line;
    while (getline(inp_file, line)) {
        // std::cout << "line: " << line << std::endl;
        std::stringstream line_stream(line);
        std::string linetype;
        line_stream >> linetype;
        if (linetype[0] == '#')
            continue;
        if (linetype == "mtllib") {
            std::string material_lib_file_name;
            line_stream >> material_lib_file_name;
            std::string rel_dir = filename;
            while (rel_dir.size() && rel_dir.back() != '/') {
                rel_dir.pop_back();
            }
            read_material_file(rel_dir + material_lib_file_name, materials);
        } else if (linetype == "v") {
            float x, y, z;
            line_stream >> x >> y >> z;
            obj_vertices.push_back({x, y, z});
        } else if (linetype == "vn") {
            float x, y, z;
            line_stream >> x >> y >> z;
            obj_normals.push_back({x, y, z});
        } else if (linetype == "vt") {
            float x, y;
            line_stream >> x >> y;
            obj_texture_coords.push_back({x, y});
        } else if (linetype == "usemtl") {
            std::string material_name;
            line_stream >> material_name;
            if (!materials.count(material_name)) {
                std::cout << "Could not find material: " << material_name
                          << std::endl;
                continue;
            }
            curr_material = materials[material_name];
        } else if (linetype == "f") {
            std::vector<std::string> face_verts_str;
            std::string face_vert_str;
            while (getline(line_stream, face_vert_str, ' ')) {
                // std::cout << "face vert: " << face_vert_str << std::endl;
                face_verts_str.push_back(face_vert_str);
            }
            std::vector<std::vector<int>> face_verts;
            for (std::string face_vert_str : face_verts_str) {
                std::stringstream face_vert_str_stream(face_vert_str);
                std::vector<int> indices;
                std::string ind_str;
                while (getline(face_vert_str_stream, ind_str, '/')) {
                    int ind = std::stoi(ind_str);
                    indices.push_back(ind);
                }
                if (indices.size() > 2)
                    face_verts.push_back(indices);
            }
            if (face_verts.size() < 3) {
                std::cout << "faces should have at least 3 vertices smh"
                          << std::endl;
            }
            int first_vert_ind;
            // std::cout << "size of obj vertices: " << obj_vertices.size()
            //           << std::endl;
            // std::cout << "size of normals: " << obj_normals.size() <<
            // std::endl;
            for (int i = 0; i < face_verts.size(); i++) {
                // std::cout << "size of face_verts[i]: " <<
                // face_verts[i].size()
                //           << std::endl;
                glm::vec3 vert = obj_vertices[face_verts[i][0] - 1];
                glm::vec3 normal(1.0f, 1.0f, 1.0f);
                glm::vec3 color = curr_material.color;
                glm::vec2 texture_coord(0.0f, 0.0f);
                if (face_verts[i].size() > 1) {
                    texture_coord = obj_texture_coords[face_verts[i][1] - 1];
                }
                if (face_verts[i].size() > 2) {
                    normal = obj_normals[face_verts[i][2] - 1];
                }
                verts.push_back(vert.x);
                verts.push_back(vert.y);
                verts.push_back(vert.z);
                verts.push_back(color.x);
                verts.push_back(color.y);
                verts.push_back(color.z);
                verts.push_back(normal.x);
                verts.push_back(normal.y);
                verts.push_back(normal.z);
                verts.push_back(texture_coord.x);
                verts.push_back(texture_coord.y);
                num_verts++;
                if (i == 0) {
                    first_vert_ind = num_verts;
                } else if (i < face_verts.size() - 1) {
                    indices.push_back(first_vert_ind - 1);
                    indices.push_back(num_verts - 1);
                    indices.push_back(num_verts);
                }
            }
        } else {
            // std::cout << "Line type: " << linetype << std::endl;
        }
    }
    assert(num_verts * NUM_FLOATS_PER_VERTEX == verts.size());
    ret->vertex_count = num_verts;
    ret->index_count = indices.size();
    ret->num_floats_per_vertex = NUM_FLOATS_PER_VERTEX;

    ret->vertices = (GLfloat *)malloc(ret->get_size_bytes());
    ret->indices = (GLushort *)malloc(ret->get_indices_size_bytes());
    memcpy(ret->vertices, verts.data(), ret->get_size_bytes());
    memcpy(ret->indices, indices.data(), ret->get_indices_size_bytes());

    return ret;
}
