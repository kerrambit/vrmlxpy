#include "VrmlFileTraversor.hpp"

#include <memory>

#include <result.hpp>

#include "ConversionContextActionMap.hpp"
#include "Error.hpp"
#include "FileTraversorError.hpp"
#include "FullParsedVrmlFileContext.hpp"
#include "FullParsedVrmlNodeContext.hpp"
#include "Logger.hpp"
#include "MeshConversionContext.hpp"
#include "NullPointerError.hpp"
#include "VrmlNodeTraversor.hpp"

#include "VrmlProcessingExport.hpp"

template<typename ConversionContext>
cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> vrml_proc::traversor::VrmlFileTraversor::Traverse(FullParsedVrmlFileContext context, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap) {
	// TODO: make parallel for-loop. VrmlNodeManager has to be made thread-safe.

	std::shared_ptr<ConversionContext> traversedFile = std::make_shared<ConversionContext>();

	for (const auto& root : context.file) {

		LOG_INFO() << "Found root node of type <" << root.header << ">.";

		cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> result =
			vrml_proc::traversor::VrmlNodeTraversor::Traverse<ConversionContext>(vrml_proc::traversor::FullParsedVrmlNodeContext(root, context.manager), actionMap);

		if (result.has_error()) {
			LOG_ERROR() << "While traversing root node <" << root.header << "> error occured!";
			auto fileError = std::make_shared<vrml_proc::traversor::error::FileTraversorError>(result.error(), root);
			return cpp::fail(fileError);
		}

		if (result.value() == nullptr) {
			LOG_ERROR() << "While traversing root node <" << root.header << "> unexpected internal error occured!";
			auto nullPointerError = std::make_shared<vrml_proc::core::error::NullPointerError>();
			auto fileError = std::make_shared<vrml_proc::traversor::error::FileTraversorError>(
				nullPointerError, root
			);
			return cpp::fail(fileError);
		}

		traversedFile->Merge(result.value().get());
	}

	return traversedFile;
}

namespace vrml_proc {
	namespace traversor {
		namespace  VrmlFileTraversor {
			template VRMLPROCESSING_API cpp::result<std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> Traverse<vrml_proc::conversion_context::MeshConversionContext>(FullParsedVrmlFileContext context, const vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext>& actionMap);
		}
	}
}