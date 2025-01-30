#include "BoxAction.hpp"

#include <functional>
#include <memory>

#include "GeometryAction.hpp"
#include "Logger.hpp"
#include "MeshConversionContext.hpp"
#include "StlBaseStructure.hpp"
#include "Vec3f.hpp"

namespace vrml_proc {
	namespace action {

		BoxAction::BoxAction(std::reference_wrapper<const vrml_proc::parser::Vec3f> size, bool containedByShape) :
			GeometryAction(containedByShape), m_size(size) {}

		std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext> BoxAction::Execute() {
			
			LOG_INFO() << "Execute BoxAction.";

			auto result = std::make_shared<vrml_proc::conversion_context::MeshConversionContext>();
			if (!m_containedByShape) {
				LOG_DEBUG() << "Return empty data because Box node is not a child of a Shape node.";
				return result;
			}

			vrml_proc::conversion_context::StlBaseStructure entity = vrml_proc::conversion_context::StlBaseStructure({ 1.0, 1.0, 1.0 }, { {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0} });
			result->Add(std::move(entity));

			return result;
		}
	}
}
