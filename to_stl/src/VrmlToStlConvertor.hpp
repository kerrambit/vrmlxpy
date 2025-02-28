#pragma once

#include <future>
#include <memory>
#include <vector>

#include <result.hpp>

#include "BufferView.hpp"
#include "CalculatorResult.hpp"
#include "Error.hpp"
#include "Mesh.hpp"
#include "MeshTaskConversionContext.hpp"
#include "StlActionMap.hpp"
#include "VrmlProcessor.hpp"

#include "VrmlToStlExport.hpp"

namespace to_stl::VrmlToStlConvertor {

  inline VRMLTOSTL_API cpp::result<std::shared_ptr<to_stl::core::Mesh>, std::shared_ptr<vrml_proc::core::error::Error>>
  Convert(vrml_proc::parser::BufferView dataToProcess) {
    auto processingResult = vrml_proc::VrmlProcessor::Process<to_stl::conversion_context::MeshTaskConversionContext>(
        dataToProcess, to_stl::conversion_context::CreateActionMap());
    if (processingResult.has_error()) {
      return cpp::fail(processingResult.error());
    }

    std::vector<std::future<to_stl::calculator::CalculatorResult>> calculatorResults;
    for (const auto& task : processingResult.value()->GetData()) {
      if (task) {
        calculatorResults.emplace_back(std::async(std::launch::async, task));
      }
    }

    std::shared_ptr<to_stl::core::Mesh> mesh;
    for (auto& future : calculatorResults) {
      auto meshResult = future.get();
      if (meshResult.has_value()) {
        mesh->join(*(meshResult.value()));
      }
    };

    return mesh;
  }
}  // namespace to_stl::VrmlToStlConvertor
