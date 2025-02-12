#include "TransformAction.hpp"

#include <memory>

#include "Logger.hpp"
#include "MeshConversionContext.hpp"

namespace to_stl::action {
		
	TransformAction::TransformAction(TransformProperties properties)
		: m_properties(properties) {}

	std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext> TransformAction::Execute() {

		vrml_proc::core::logger::LogInfo("Execute TransformAction.", LOGGING_INFO);

		auto result = std::make_shared<vrml_proc::conversion_context::MeshConversionContext>();

		for (auto child : m_properties.children) {
			result->Merge(child.get());
			child.reset();
		}

		return result;
	}
}
