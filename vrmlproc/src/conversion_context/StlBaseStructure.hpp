#pragma once

#include "Vec3f.hpp"

namespace vrml_proc {
	namespace conversion_context {

		struct StlBaseStructure {

			vrml_proc::parser::Vec3f facetNormal;

			struct OuterLoopVertices {

				OuterLoopVertices(vrml_proc::parser::Vec3f x, vrml_proc::parser::Vec3f y, vrml_proc::parser::Vec3f z)
					: x(x), y(y), z(z) {}

				vrml_proc::parser::Vec3f x;
				vrml_proc::parser::Vec3f y;
				vrml_proc::parser::Vec3f z;
			};

			OuterLoopVertices outerLoopVertices;

			StlBaseStructure(vrml_proc::parser::Vec3f facetNormal, const OuterLoopVertices& outerLoopVertices)
				: facetNormal(facetNormal), outerLoopVertices(outerLoopVertices) {}
		};
	}
}
