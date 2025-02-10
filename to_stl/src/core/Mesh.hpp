#pragma once

#include <CGAL/Kernel/interface_macros.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh/Surface_mesh.h>

#include "Point.hpp"

namespace to_stl {
	namespace core {
		using Mesh = CGAL::Surface_mesh<vrml_proc::math::utils::CGALPoint>;
	}
}
