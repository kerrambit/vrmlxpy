#include "VrmlFileTraversor.hpp"

#include <memory>

#include "BaseConversionContextActionMap.hpp"
#include "FullParsedVrmlFileContext.hpp"
#include "FullParsedVrmlNodeContext.hpp"
#include "MeshConversionContext.hpp"
#include "VrmlNodeTraversor.hpp"

#include "VrmlProcessingExport.hpp"

template<typename ConversionContext>
std::shared_ptr<ConversionContext> vrml_proc::traversor::VrmlFileTraversor::Traverse(FullParsedVrmlFileContext context, const vrml_proc::action::BaseConversionContextActionMap<ConversionContext>& actionMap) {
	// TODO: make parallel for-loop. VrmlNodeManager has to be made thread-safe.

	std::shared_ptr<ConversionContext> traversedFile = std::make_shared<ConversionContext>();

	for (const auto& root : context.file) {

		std::cout << "Found root node of type " << root.header << std::endl;
		std::shared_ptr<ConversionContext> result =
			vrml_proc::traversor::VrmlNodeTraversor::Traverse<ConversionContext>(vrml_proc::traversor::FullParsedVrmlNodeContext(root, context.manager), actionMap);

		if (result == nullptr) {
			return result;
		}

		traversedFile->Merge(result.get());
	}

	return traversedFile;
}

namespace vrml_proc {
	namespace traversor {
		namespace  VrmlFileTraversor {
			template VRMLPROCESSING_API std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext> Traverse<vrml_proc::conversion_context::MeshConversionContext>(FullParsedVrmlFileContext context, const vrml_proc::action::BaseConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext>& actionMap);
		}
	}
}