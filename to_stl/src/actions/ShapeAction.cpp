#include "ShapeAction.hpp"

#include <memory>

#include "Logger.hpp"
#include "MeshTaskConversionContext.hpp"

namespace to_stl {
	namespace action {
	
		ShapeAction::ShapeAction(std::shared_ptr<to_stl::conversion_context::MeshTaskConversionContext> appearance, std::shared_ptr<to_stl::conversion_context::MeshTaskConversionContext> geometry) :
			m_appearance(appearance), m_geometry(geometry) {}

		std::shared_ptr<to_stl::conversion_context::MeshTaskConversionContext> ShapeAction::Execute() {
			vrml_proc::core::logger::LogInfo("Execute ShapeAction.", LOGGING_INFO);
			return m_geometry;
		}
	}
}
