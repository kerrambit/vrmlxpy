#include "TransformAction.hpp"

#include <memory>

#include "Logger.hpp"
#include "MeshTaskConversionContext.hpp"

namespace to_stl::action {

  TransformAction::TransformAction(TransformProperties properties) : m_properties(properties) {}

  std::shared_ptr<to_stl::conversion_context::MeshTaskConversionContext> TransformAction::Execute() {
    vrml_proc::core::logger::LogInfo("Execute TransformAction.", LOGGING_INFO);

    auto result = std::make_shared<to_stl::conversion_context::MeshTaskConversionContext>();

    for (auto child : m_properties.children) {
      result->Merge(child.get());
      child.reset();
    }

    return result;
  }
}  // namespace to_stl::action
