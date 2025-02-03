#pragma once

#include <memory> 
#include <iostream>
#include <fstream>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point_2;
typedef Kernel::Point_3 Point;
typedef CGAL::Surface_mesh<Point> Mesh;

void test() {
    Point_2 p(1, 1), q(2, 2);
    std::cout << "Distance: " << CGAL::squared_distance(p, q) << std::endl;
}

std::shared_ptr<Mesh> create_box_mesh(double size_x, double size_y, double size_z) {

    auto mesh = std::make_shared<Mesh>();

    double half_x = size_x / 2.0;
    double half_y = size_y / 2.0;
    double half_z = size_z / 2.0;

    Point vertices[8] = {
        Point(-half_x, -half_y, -half_z),
        Point(half_x, -half_y, -half_z),
        Point(half_x,  half_y, -half_z),
        Point(-half_x,  half_y, -half_z),
        Point(-half_x, -half_y,  half_z),
        Point(half_x, -half_y,  half_z),
        Point(half_x,  half_y,  half_z),
        Point(-half_x,  half_y,  half_z)
    };

    Mesh::Vertex_index v[8];
    for (int i = 0; i < 8; ++i) {
        v[i] = mesh->add_vertex(vertices[i]);
    }

    // Front face.
    mesh->add_face(v[4], v[5], v[6]);
    mesh->add_face(v[4], v[6], v[7]);

    // Back face.
    mesh->add_face(v[1], v[0], v[3]);
    mesh->add_face(v[1], v[3], v[2]);

    // Top face.
    mesh->add_face(v[7], v[6], v[2]);
    mesh->add_face(v[7], v[2], v[3]);

    // Bottom face.
    mesh->add_face(v[0], v[1], v[5]);
    mesh->add_face(v[0], v[5], v[4]);

    // Right face.
    mesh->add_face(v[5], v[1], v[2]);
    mesh->add_face(v[5], v[2], v[6]);

    // Left face.
    mesh->add_face(v[0], v[4], v[7]);
    mesh->add_face(v[0], v[7], v[3]);

    return mesh;
}

#include <CGAL/boost/graph/IO/STL.h>

bool export_to_stl(const Mesh& mesh, const std::string& filename) {
    return CGAL::IO::write_STL(filename, mesh);
}