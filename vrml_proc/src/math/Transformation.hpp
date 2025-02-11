#pragma once

#include "Vec3f.hpp"
#include "Vec4f.hpp"

using vrml_proc::parser::Vec3f;
using vrml_proc::parser::Vec4f;

namespace vrml_proc::math {
	struct Transformation {
		Vec3f center;
		Vec4f rotation;
		Vec3f scale;
		Vec4f scaleOrientation;
		Vec3f translation;

		Transformation()
			: center(Vec3f()), rotation(Vec4f(0.0f, 0.0f, 1.0f, 0.0f)), scale(Vec3f(1.0f, 1.0f, 1.0f)), scaleOrientation(Vec4f(0.0f, 0.0f, 1.0f, 0.0f)), translation(Vec3f()) {}
	};
}
