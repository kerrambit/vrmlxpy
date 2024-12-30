#pragma once

#include <memory>

#include "BaseConversionContext.hpp"
#include "BaseConversionContextActionMap.hpp"
#include "MeshConversionContext.hpp"
#include "VrmlFile.hpp"
#include "VrmlNodeManager.hpp"
#include "VrmlNodeTraversor.hpp"
#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
	namespace traversor {
		struct VRMLPROCESSING_API FullParsedVrmlFileContext {

			FullParsedVrmlFileContext(const vrml_proc::parser::VrmlFile& file, const vrml_proc::parser::VrmlNodeManager& manager)
				: file(file), manager(manager) {}

			const vrml_proc::parser::VrmlFile& file;
			const vrml_proc::parser::VrmlNodeManager& manager;
		};

		class VRMLPROCESSING_API VrmlFileTraversor {
		public:
			static std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> Traverse(FullParsedVrmlFileContext context, const vrml_proc::action::BaseConversionContextActionMap& actionMap) {

				// TODO: make parallel for-loop. VrmlNodeManager has to be made thread-safe.

				std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> traversedFile = std::make_shared<vrml_proc::conversion_context::MeshConversionContext>();

				for (const auto& root : context.file) {

					std::cout << "Found root node of type " << root.header << std::endl;
					std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> result =
						vrml_proc::traversor::VrmlNodeTraversor::Traverse(vrml_proc::traversor::FullParsedVrmlNodeContext(root, context.manager), actionMap);
					
					traversedFile->Merge(result.get());
				}

				return traversedFile;
			}
		};
	}
}
