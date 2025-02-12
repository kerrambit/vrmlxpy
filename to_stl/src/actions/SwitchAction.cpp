#include "SwitchAction.hpp"

#include <memory>

#include "Logger.hpp"
#include "MeshConversionContext.hpp"

namespace to_stl::action {

	SwitchAction::SwitchAction(SwitchProperties properties)
		: m_properties(properties) {}

	std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext> SwitchAction::Execute() {

		vrml_proc::core::logger::LogInfo("Execute SwitchAction.", LOGGING_INFO);

		auto result = std::make_shared<vrml_proc::conversion_context::MeshConversionContext>();
		result->Merge(m_properties.choice.get());
		m_properties.choice.reset();

		return result;
	}
}
