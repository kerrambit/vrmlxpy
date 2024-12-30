#pragma once

#include "Vec3f.hpp"
#include "Printable.hpp"

namespace vrml_proc {
	namespace conversion_context {
		/**
		 * @brief Representation of STL file, or rather a basic data entity in it.
		 * 
		 * @implements Printable
		 */
		struct StlBaseStructure : public Printable {
			/**
			 * @brief Internal StlBaseStructure structure representing a part of basic STL data entity.
			 */
			struct OuterLoopVertices {
				/**
				 * @brief Default constructor.
				 * 
				 * @param x first vector `Vec3f`
				 * @param y second vector `Vec3f`
				 * @param z third vector `Vec3f` 
				 */
				OuterLoopVertices(vrml_proc::parser::Vec3f x, vrml_proc::parser::Vec3f y, vrml_proc::parser::Vec3f z)
					: x(x), y(y), z(z) {}

				vrml_proc::parser::Vec3f x;
				vrml_proc::parser::Vec3f y;
				vrml_proc::parser::Vec3f z;
			};

			vrml_proc::parser::Vec3f facetNormal;
			OuterLoopVertices outerLoopVertices;

			/**
			 * @brief Default constructor.
			 * 
			 * @param facetNormal
			 * @param outerLoopVertices
			 */
			StlBaseStructure(vrml_proc::parser::Vec3f facetNormal, const OuterLoopVertices& outerLoopVertices)
				: Printable(std::cout), facetNormal(facetNormal), outerLoopVertices(outerLoopVertices) {}

			/**
			 * @brief Overriden implementation of interface method. Prints the StlBaseStructure.
			 * 
			 * @todo should be moved into .cpp file
			 * @param indentationLevel indentation level for printing this object
			 */
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
