#pragma once

#include <CGAL/boost/graph/IO/STL.h>
#include <CGAL/Kernel/interface_macros.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh/Surface_mesh.h>

#include "Mesh.hpp"

bool export_to_stl(const to_stl::core::Mesh& mesh, const std::string& filename) {
    return CGAL::IO::write_STL(filename, mesh);
}
