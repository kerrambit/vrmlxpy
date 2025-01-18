#include "VrmlFileTraversor.hpp"

#include <memory>

#include "BaseConversionContext.hpp"
#include "BaseConversionContextActionMap.hpp"
#include "MeshConversionContext.hpp"
#include "VrmlNodeTraversor.hpp"

#include "VrmlProcessingExport.hpp"

template<typename DefaultConversionContext>
std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> vrml_proc::traversor::VrmlFileTraversor::Traverse(FullParsedVrmlFileContext context, const vrml_proc::action::BaseConversionContextActionMap& actionMap) {
	// TODO: make parallel for-loop. VrmlNodeManager has to be made thread-safe.

	std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> traversedFile = std::make_shared<DefaultConversionContext>();

	for (const auto& root : context.file) {

		std::cout << "Found root node of type " << root.header << std::endl;
		std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> result =
			vrml_proc::traversor::VrmlNodeTraversor::Traverse<DefaultConversionContext>(vrml_proc::traversor::FullParsedVrmlNodeContext(root, context.manager), actionMap);

		traversedFile->Merge(result.get());
	}

	return traversedFile;
}

namespace vrml_proc {
	namespace traversor {
		namespace  VrmlFileTraversor {
			template VRMLPROCESSING_API std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> Traverse<vrml_proc::conversion_context::MeshConversionContext>(FullParsedVrmlFileContext context, const vrml_proc::action::BaseConversionContextActionMap& actionMap);
		}
	}
}