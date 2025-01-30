#pragma once

#include "Printable.hpp"
#include "Vec3f.hpp"

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
					: x(std::move(x)), y(std::move(y)), z(std::move(z)) {}

				vrml_proc::parser::Vec3f x;
				vrml_proc::parser::Vec3f y;
				vrml_proc::parser::Vec3f z;

				OuterLoopVertices(OuterLoopVertices&& other) noexcept
					: x(std::exchange(other.x, vrml_proc::parser::Vec3f())),
					  y(std::exchange(other.y, vrml_proc::parser::Vec3f())),
					  z(std::exchange(other.z, vrml_proc::parser::Vec3f())) {}

				OuterLoopVertices& operator=(OuterLoopVertices&& other) noexcept {
					if (this != &other) {
						x = std::move(other.x);
						y = std::move(other.y);
						z = std::move(other.z);
					}
					return *this;
				}

				OuterLoopVertices(const OuterLoopVertices&) = default;

				OuterLoopVertices& operator=(const OuterLoopVertices&) = default;
			};

			vrml_proc::parser::Vec3f facetNormal;
			OuterLoopVertices outerLoopVertices;

			/**
			 * @brief Default constructor.
			 * 
			 * @param facetNormal
			 * @param outerLoopVertices
			 */
			StlBaseStructure(vrml_proc::parser::Vec3f facetNormal, OuterLoopVertices outerLoopVertices)
				: Printable(std::cout), facetNormal(std::move(facetNormal)), outerLoopVertices(std::move(outerLoopVertices)) {}

			StlBaseStructure(StlBaseStructure&& other) noexcept
				: Printable(std::cout),
				facetNormal(std::move(other.facetNormal)),
				outerLoopVertices(std::move(other.outerLoopVertices)) {}

			StlBaseStructure& operator=(StlBaseStructure&& other) noexcept {
				if (this != &other) {
					facetNormal = std::move(other.facetNormal);
					outerLoopVertices = std::move(other.outerLoopVertices);
				}
				return *this;
			}

			StlBaseStructure(const StlBaseStructure&) = default;

			StlBaseStructure& operator=(const StlBaseStructure&) = default;

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
