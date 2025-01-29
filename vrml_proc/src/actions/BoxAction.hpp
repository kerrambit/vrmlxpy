#pragma once

#include <memory>

#include "ConversionContextAction.hpp"
#include "GeometryAction.hpp"
#include "MeshConversionContext.hpp"
#include "StlBaseStructure.hpp"
#include "Vec3f.hpp"

namespace vrml_proc {
	namespace action {

		class BoxAction : public GeometryAction {

		public:
			BoxAction(std::reference_wrapper<const vrml_proc::parser::Vec3f> size, bool containedByShape) :
				GeometryAction(containedByShape), m_size(size) {}

			// TODO: use here some mesh calculator to create data for STL
			std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext> Execute() override {
				std::cout << "Action: " << m_size.get() << std::endl;
				auto result = std::make_shared<vrml_proc::conversion_context::MeshConversionContext>();
				if (!m_containedByShape) {
					return result;
				}
				vrml_proc::conversion_context::StlBaseStructure entity = vrml_proc::conversion_context::StlBaseStructure({ 1.0, 1.0, 1.0 }, { {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0} });
				result->Add(entity);
				return result;
			}
		private:
			std::reference_wrapper<const vrml_proc::parser::Vec3f> m_size;
		};
	}
}
