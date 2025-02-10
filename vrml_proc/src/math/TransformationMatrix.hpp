#pragma once

#include <CGAL/Aff_transformation_3.h>
#include <CGAL/Kernel/interface_macros.h>
#include <CGAL/Kernel/Type_equality_wrapper.h>
#include <CGAL/Simple_cartesian.h>

#include "Point.hpp"
#include "Transformation.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc::math {

	using TransformationMatrix = CGAL::Aff_transformation_3<vrml_proc::math::utils::CGALKernel>;

	VRMLPROCESSING_API void UpdateTransformationMatrix(TransformationMatrix& matrix, const Transformation& transformationData);
}
