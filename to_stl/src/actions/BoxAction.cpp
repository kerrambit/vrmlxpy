#include "BoxAction.hpp"

#include <functional>
#include <memory>

#include "GeometryAction.hpp"
#include "Logger.hpp"
#include "MeshConversionContext.hpp"
#include "StlBaseStructure.hpp"
#include "Vec3f.hpp"

namespace to_stl {
	namespace action {

		BoxAction::BoxAction(std::reference_wrapper<const vrml_proc::parser::Vec3f> size, bool containedByShape) :
			vrml_proc::action::GeometryAction(containedByShape), m_size(size) {}

		std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext> BoxAction::Execute() {
			
			vrml_proc::core::logger::LogInfo("Execute BoxAction.", LOGGING_INFO);

			auto result = std::make_shared<vrml_proc::conversion_context::MeshConversionContext>();
			if (!m_containedByShape) {
				vrml_proc::core::logger::LogDebug("Return empty data because Box node is not a child of a Shape node.", LOGGING_INFO);
				return result;
			}

			result->Add(vrml_proc::conversion_context::StlBaseStructure({ 1.0, 1.0, 1.0 }, { {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0} }));
			return result;
		}
	}
}
