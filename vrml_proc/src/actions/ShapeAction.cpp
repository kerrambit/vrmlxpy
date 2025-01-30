#include "ShapeAction.hpp"

#include <memory>

#include "Logger.hpp"
#include "MeshConversionContext.hpp"

namespace vrml_proc {
	namespace action {
	
		ShapeAction::ShapeAction(std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext> appearance, std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext> geometry) :
			m_appearance(appearance), m_geometry(geometry) {}

		std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext> ShapeAction::Execute() {
			LOG_INFO() << "Execute ShapeAction.";
			return m_geometry;
		}
	}
}
