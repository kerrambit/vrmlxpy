#include "SwitchAction.hpp"

#include <memory>

#include "Logger.hpp"
#include "MeshTaskConversionContext.hpp"

namespace to_stl::action {

  SwitchAction::SwitchAction(SwitchProperties properties) : m_properties(properties) {}

  std::shared_ptr<to_stl::conversion_context::MeshTaskConversionContext> SwitchAction::Execute() {
    vrml_proc::core::logger::LogInfo("Execute SwitchAction.", LOGGING_INFO);

    auto result = std::make_shared<to_stl::conversion_context::MeshTaskConversionContext>();
    result->Merge(m_properties.choice.get());
    m_properties.choice.reset();

    return result;
  }
}  // namespace to_stl::action
