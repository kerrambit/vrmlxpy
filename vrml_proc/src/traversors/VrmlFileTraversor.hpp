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
#include "ConversionContextActionMap.hpp"
#include "Error.hpp"
#include "FileTraversorError.hpp"
#include "FullParsedVrmlFileContext.hpp"
#include "FullParsedVrmlNodeContext.hpp"
#include "Logger.hpp"
#include "MeshConversionContext.hpp"
#include "NullPointerError.hpp"
#include "VrmlNodeTraversor.hpp"
#include "FormatString.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
	namespace traversor {
		namespace  VrmlFileTraversor {
			template<typename ConversionContext>
			VRMLPROCESSING_API inline cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> Traverse(const FullParsedVrmlFileContext& context, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap) {

				// TODO: make parallel for-loop. VrmlNodeManager has to be made thread-safe.

				std::shared_ptr<ConversionContext> traversedFile = std::make_shared<ConversionContext>();

				for (const auto& root : context.file) {

					vrml_proc::core::logger::LogInfo(vrml_proc::core::utils::FormatString("Found root node of type <", root.header, ">."), LOGGING_INFO);

					cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> result =
						vrml_proc::traversor::VrmlNodeTraversor::Traverse<ConversionContext>(vrml_proc::traversor::FullParsedVrmlNodeContext(root, context.manager, false, vrml_proc::math::TransformationMatrix()), actionMap);

					if (result.has_error()) {
						vrml_proc::core::logger::LogError(vrml_proc::core::utils::FormatString("While traversing root node <", root.header, "> error occured!"), LOGGING_INFO);
						auto fileError = std::make_shared<vrml_proc::traversor::error::FileTraversorError>(result.error(), root);
						return cpp::fail(fileError);
					}

					if (result.value() == nullptr) {
						vrml_proc::core::logger::LogError(vrml_proc::core::utils::FormatString("While traversing root node <", root.header, "> unxpected internal error occured!"), LOGGING_INFO);
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
		}
	}
}
