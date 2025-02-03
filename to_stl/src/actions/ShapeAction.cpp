#include "ShapeAction.hpp"

#include <memory>

#include "Logger.hpp"
#include "MeshConversionContext.hpp"

namespace to_stl {
	namespace action {
	
		ShapeAction::ShapeAction(std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext> appearance, std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext> geometry) :
			m_appearance(appearance), m_geometry(geometry) {}

		std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext> ShapeAction::Execute() {
			vrml_proc::core::logger::LogInfo("Execute ShapeAction.", LOGGING_INFO);
			return m_geometry;
		}
	}
}
