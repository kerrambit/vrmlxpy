#pragma once

#include <filesystem>
#include <memory>

#include <result.hpp>

#include "BufferView.hpp"
#include "ConversionContextActionMap.hpp"
#include "Error.hpp"
#include "ParserError.hpp"
#include "VrmlNodeManager.hpp"
#include "VrmlParser.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc::VrmlProcessor {

	template<typename ConversionContext>
	VRMLPROCESSING_API inline cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>>
		Process(vrml_proc::parser::BufferView& dataToProcess, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap) {
			
		vrml_proc::parser::VrmlNodeManager manager;
		vrml_proc::parser::VrmlParser parser(manager);

		auto parseResult = parser.Parse(dataToProcess);
		if (parseResult.has_value()) {
			return vrml_proc::traversor::VrmlFileTraversor::Traverse<ConversionContext>({ parseResult.value(), manager }, actionMap);
		}

		return cpp::fail(parseResult.error());
	}
}
