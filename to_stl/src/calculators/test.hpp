#pragma once

#include <CGAL/boost/graph/IO/STL.h>
#include <CGAL/Kernel/interface_macros.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh/Surface_mesh.h>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_3 Point;
typedef CGAL::Surface_mesh<Point> Mesh;

bool export_to_stl(const Mesh& mesh, const std::string& filename) {
    return CGAL::IO::write_STL(filename, mesh);
}
