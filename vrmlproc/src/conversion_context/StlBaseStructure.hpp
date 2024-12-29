#pragma once

#include "Vec3f.hpp"
#include "Printable.hpp"

namespace vrml_proc {
	namespace conversion_context {

		struct StlBaseStructure : public Printable {

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
				: Printable(std::cout), facetNormal(facetNormal), outerLoopVertices(outerLoopVertices) {}

			// TODO: move into .cpp file
			void Print(Printable::IndentationLevel indentationLevel) const override {
				std::string indentationString = Printable::CreateIndentationString(indentationLevel);
				indentationLevel++;

				*Printable::AccessStreamPointer() << indentationString;
				*Printable::AccessStreamPointer() << "StlBaseStructure:\n";
				*Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentationLevel) << "Facet Normal: \n";
				*Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentationLevel + 1) << facetNormal << "\n";
				*Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentationLevel) << "Outer Loop Vertices: " << "\n";
				*Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentationLevel + 1) << outerLoopVertices.x << "\n";
				*Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentationLevel + 1) << outerLoopVertices.y << "\n";
				*Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentationLevel + 1) << outerLoopVertices.z << "\n";

				*Printable::AccessStreamPointer() << std::endl;
			}
		};
	}
}
