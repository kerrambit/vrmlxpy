#pragma once

#include <memory>

#include "BaseConversionContext.hpp"
#include "ConversionContextActionMap.hpp"
#include "FullParsedVrmlFileContext.hpp"
#include "MeshConversionContext.hpp"
#include "VrmlFile.hpp"
#include "VrmlNodeManager.hpp"
#include "VrmlNodeTraversor.hpp"
#include "Error.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
	namespace traversor {
		namespace  VrmlFileTraversor {
			template<typename ConversionContext>
			VRMLPROCESSING_API cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> Traverse(const FullParsedVrmlFileContext& context, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap);
		}
	}
}
