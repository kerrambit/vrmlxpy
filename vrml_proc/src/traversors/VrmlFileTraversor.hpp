#pragma once

#include <memory>

#include "BaseConversionContext.hpp"
#include "ConversionContextActionMap.hpp"
#include "FullParsedVrmlFileContext.hpp"
#include "MeshConversionContext.hpp"
#include "VrmlFile.hpp"
#include "VrmlNodeManager.hpp"
#include "VrmlNodeTraversor.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
	namespace traversor {
		namespace  VrmlFileTraversor {
			template<typename ConversionContext>
			VRMLPROCESSING_API std::shared_ptr<ConversionContext> Traverse(FullParsedVrmlFileContext context, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap);
		}
	}
}
